/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/20 09:07:58 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "external.h"
#include "minishell.h"

/*
** 環境変数配列の解放
*/
void	free_env_array(char **env_array, int count)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (i < count && env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

/*
** 環境変数リスト内のノード数をカウント
*/
int	count_env_nodes(t_env *env_list)
{
	t_env	*current;
	int		count;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/*
** 子プロセスでのexecve失敗時の処理
*/
int	handle_child_process(char *cmd_path, char **args)
{
	setup_child_signals();
	launch_child(cmd_path, args);
	if (errno == EACCES)
	{
		perror(cmd_path);
		exit(126);
	}
	if (errno == ENOENT)
	{
		perror(cmd_path);
		exit(127);
	}
	perror(cmd_path);
	exit(1);
}

/*
** コマンドパスがディレクトリか判定
*/
int	check_if_directory(char *cmd_path, char *cmd_name)
{
	struct stat	st;

	if (stat(cmd_path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			ft_printf_fd(2, "minishell: %s: Is a directory\n", cmd_name);
			return (126);
		}
		return (0);
	}
	return (handle_stat_error(cmd_name));
}

/* execute_external_commandはexternal_commands_exec.cへ移動 */
