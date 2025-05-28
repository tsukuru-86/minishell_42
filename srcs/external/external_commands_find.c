/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands_find.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 01:13:11 by muiida            #+#    #+#             */
/*   Updated: 2025/05/28 20:42:12 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "external.h"
#include "minishell.h"

/* Get the PATH environment variable string */
static char	*get_path_env_value(void)
{
	t_env	*path_env_node;

	path_env_node = get_env_node("PATH");
	if (path_env_node && path_env_node->value)
		return (path_env_node->value);
	else
		return (NULL);
}

/* PATH環境変数文字列から次のパスセグメントを抽出する
   current_path_start は呼び出し元で更新されるようにポインタのポインタで渡す
   path_segment は結果を格納するバッファ */
static bool	extract_next_path_segment(const char **current_path_start,
		char *path_segment_buf, size_t buf_size)
{
	const char	*colon;
	size_t		segment_len;

	if (!*current_path_start || !**current_path_start)
		return (false);
	colon = ft_strchr(*current_path_start, ':');
	if (colon)
	{
		segment_len = colon - *current_path_start;
		if (segment_len >= buf_size)
			segment_len = buf_size - 1;
		ft_memcpy(path_segment_buf, *current_path_start, segment_len);
		path_segment_buf[segment_len] = '\0';
		*current_path_start = colon + 1;
	}
	else
	{
		ft_strlcpy(path_segment_buf, *current_path_start, buf_size);
		*current_path_start = NULL;
	}
	return (true);
}

static char	*build_and_validate_full_path(const char *path_segment,
		const char *cmd)
{
	char		full_path[PATH_MAX];
	struct stat	st;
	size_t		path_segment_len;

	if (!path_segment || path_segment[0] == '\0')
		return (NULL);
	ft_strlcpy(full_path, path_segment, PATH_MAX);
	path_segment_len = ft_strlen(full_path);
	if (path_segment_len > 0 && full_path[path_segment_len - 1] != '/')
	{
		if (ft_strlcat(full_path, "/", PATH_MAX) >= PATH_MAX)
			return (NULL);
	}
	if (ft_strlcat(full_path, cmd, PATH_MAX) >= PATH_MAX)
		return (NULL);
	if (access(full_path, X_OK) == 0)
	{
		if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode))
			return (ft_strdup(full_path));
	}
	return (NULL);
}

static char	*search_in_path(const char *path_env, const char *cmd)
{
	const char	*current_path_ptr;
	char		path_segment_buf[PATH_MAX];
	char		*executable_path;

	if (!path_env || !cmd)
		return (NULL);
	current_path_ptr = path_env;
	while (current_path_ptr)
	{
		if (!extract_next_path_segment(&current_path_ptr, path_segment_buf,
				PATH_MAX))
			break ;
		executable_path = build_and_validate_full_path(path_segment_buf, cmd);
		if (executable_path)
			return (executable_path);
	}
	return (NULL);
}

/* コマンドのパスを検索する関数。
   絶対パスまたは相対パスの場合はそのまま、
   単純なコマンド名の場合はPATH環境変数から検索する */
char	*find_command_path(char *cmd)
{
	char	*path_env_str;
	char	*executable_path;

	if (!cmd || cmd[0] == '\0')
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		executable_path = ft_strdup(cmd);
		if (!executable_path)
			perror("minishell: ft_strdup");
		return (executable_path);
	}
	path_env_str = get_path_env_value();
	if (!path_env_str)
		return (NULL);
	executable_path = search_in_path(path_env_str, cmd);
	return (executable_path);
}
