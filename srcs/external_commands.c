/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/20 22:24:50 by muiida            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Get the PATH environment variable string */
static char	*get_path_env_value(void)
// Renamed to avoid conflict if get_path_env is global
{
	t_env *path_env_node;

	path_env_node = get_env_node("PATH"); // from env_utils.c
	if (path_env_node && path_env_node->value)
		return (path_env_node->value);
	else
		return (NULL);
}

// search_in_path function needs to be defined.
// Example (can be static in this file or in a util file):

static char	*search_in_path(const char *path_env, const char *cmd)
{
	const char	*current_path_start;
	const char	*colon;
	char		full_path[PATH_MAX];
	struct stat	st;
	size_t		segment_len;

	char path_segment[PATH_MAX]; // Buffer for individual path segments
	if (!path_env || !cmd)
		return (NULL);
	current_path_start = path_env;
	while (current_path_start && *current_path_start)
	{
		colon = ft_strchr(current_path_start, ':');
		if (colon)
		{
			segment_len = colon - current_path_start;
			// Ensure segment_len is not too large for path_segment buffer
			if (segment_len >= PATH_MAX)
				// Prevent buffer overflow for path_segment
				segment_len = PATH_MAX - 1;
			ft_memcpy(path_segment, current_path_start, segment_len);
			path_segment[segment_len] = '\0';
		}
		else
		{
			// Last segment or only one segment
			ft_strlcpy(path_segment, current_path_start, PATH_MAX);
		}
		// Skip empty path segments (e.g.,
		// result of "::" or leading/trailing ":")
		if (ft_strlen(path_segment) > 0)
		{
			ft_strlcpy(full_path, path_segment, PATH_MAX);
			// Ensure there's enough space for '/' and cmd
			if (full_path[ft_strlen(full_path) - 1] != '/')
			{
				if (ft_strlcat(full_path, "/", PATH_MAX) >= PATH_MAX)
				{
					// Path became too long, skip this segment
					current_path_start = colon ? colon + 1 : NULL;
					continue ;
				}
			}
			if (ft_strlcat(full_path, cmd, PATH_MAX) >= PATH_MAX)
			{
				// Path became too long, skip this segment
				current_path_start = colon ? colon + 1 : NULL;
				continue ;
			}
			if (access(full_path, X_OK) == 0)
			{
				if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode))
				{
					// No need to free path_env as it's const char *
					return (ft_strdup(full_path));
				}
			}
		}
		current_path_start = colon ? colon + 1 : NULL;
	}
	// No need to free path_env
	return (NULL);
}

/* コマンドのパスを検索する関数。
   絶対パスまたは相対パスの場合はそのまま、
   単純なコマンド名の場合はPATH環境変数から検索する */
// search_in_path needs to be defined, possibly in this file if static,
// or in a utils file and declared in minishell.h
// For now, assume search_in_path is available.
char	*find_command_path(char *cmd)
// Renamed to avoid conflict if find_command is global
{
	char *path_env_str;
	char *executable_path;

	if (!cmd || cmd[0] == '\0') // Handle empty command
		return (NULL);
	if (ft_strchr(cmd, '/')) // If cmd contains a slash, it's a path
	{
		// Check if it's executable and a file (not a directory)
		// For simplicity, strdup and let execve handle errors.
		// Proper check: access(cmd,
		//	X_OK) and stat to check if it's a regular file.
		executable_path = ft_strdup(cmd);
		if (!executable_path)
			perror("minishell: ft_strdup");
		return (executable_path);
	}
	path_env_str = get_path_env_value();
	if (!path_env_str) // No PATH set,
		// or PATH is empty
		return (NULL);
	// Command not found if not a path and no PATH
	executable_path = search_in_path(path_env_str, cmd);
	// search_in_path needs definition
	// Returns allocated string or NULL
	return (executable_path);
}

/* 外部コマンドを実行する関数。
   コマンドパスを検索し、子プロセスでコマンドを実行する */
int	execute_external_command(char **args)
{
	pid_t	pid;
	char	*cmd_path;

	if (!args || !args[0])
		return (0); // Or some error for empty command
	cmd_path = find_command_path(args[0]);
	if (!cmd_path)
	{
		ft_putstr_fd((char *)"minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd((char *)": command not found\n", 2);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		free(cmd_path);
		return (1); // Fork error
	}
	if (pid == 0)
	{
		// Child process
		launch_child(cmd_path, args);
		// cmd_path will be freed in launch_child on error or by execve
		// launch_child calls exit(),
		// so this part is not reached in child on success / error of execve
	}
	// Parent process
	return (launch_parent(pid, cmd_path));
	// cmd_path will be freed in launch_parent
}
