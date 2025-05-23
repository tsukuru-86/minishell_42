/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 01:51:00 by muiida           #+#    #+#             */
/*   Updated: 2025/05/19 01:51:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 外部コマンドの子プロセスでの実行部分 */
void	execute_child_process(t_command *cmd)
{
	int	status;

	setup_child_signals();
	status = execute_external_command(cmd->args);
	exit(status);
}

/* 外部コマンドを実行する関数 */
int	execute_external_with_fork(t_command *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
		execute_child_process(cmd);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}
