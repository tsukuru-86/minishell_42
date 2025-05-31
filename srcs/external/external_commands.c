/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/01 02:34:04 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "external.h"
#include "minishell.h"
#include "../error/error_messages.h"

void	launch_child(char *cmd_path, char **args)
{
	char	**env_array;
	int		env_count;

	env_array = env_list_to_array();
	if (!env_array)
	{
		perror("minishell: env_list_to_array failed");
		free(cmd_path);
		exit(127);
	}
	if (execve(cmd_path, args, env_array) == -1)
	{
		perror("minishell: execve");
		free(cmd_path);
		env_count = count_env_nodes(*get_env_val());
		free_env_array(env_array, env_count);
		if (errno == ENOENT)
			exit(127);
		if (errno == EACCES)
			exit(126);
		exit(1);
	}
}

/* 外部コマンドを実行する関数。
   コマンドパスを検索し、子プロセスでコマンドを実行する */
int	execute_external_command(t_command *cmd)
{
	pid_t	pid;
	char	*cmd_path;
	char	**args;

	args = cmd->args;
	if (!args || !args[0])
		return (0);
	cmd_path = find_command_path(args[0]);
	if (!cmd_path)
	{
		ft_printf_fd(2, ERR_COMMAND_NOT_FOUND, args[0]);
				return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
				free(cmd_path);
		return (1);
	}
	if (pid == 0)
	{
		setup_child_signals();
		launch_child(cmd_path, args);
		exit(execute_external_command(cmd));
	}
	return (wait_parent(pid, cmd_path));
}

