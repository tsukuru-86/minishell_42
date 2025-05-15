/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 05:02:31 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/15 06:57:46 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

/* 外部コマンドの実行 */
static int	execute_external_command_in_child(t_command *cmd)
{
	int	status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	status = execute_external_command(cmd);
	exit(status);
}

/* 外部コマンドの実行処理 */
static int	execute_external_cmd(t_command *cmd)
{
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
		execute_external_command_in_child(cmd);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

/* 単一コマンドの実行 */
static int	execute_single_command(t_command *cmd)
{
	int	status;

	if (!cmd || !cmd->args)
	{
		ft_printf_fd(2, "ERROR: NULL COMMAND OR ARGS\n");
		return (1);
	}
	ft_printf_fd(2, "DEBUG: EXECUTING COMMAND: %s\n", cmd->args[0]);
	if (cmd->redirects && !setup_redirection(cmd->redirects))
		return (1);
	if (is_builtin(cmd->args[0]))
		status = execute_builtin(cmd);
	else
		status = execute_external_cmd(cmd);
	if (cmd->redirects)
		restore_redirection(cmd->redirects);
	return (status);
}

/*
** NOTE: This function is no longer used with our new pipeline implementation.
** The functionality has been moved to execute_pipeline_command in pipeline.c
*/
#if 0
/* コマンドの実行（子プロセス用） */
static int	execute_command_in_child(t_command *cmd, char **envp)
{
	int	status;

	if (!cmd || !cmd->args || !cmd->args[0])
		exit(0);
	// パイプライン実行中であることを示す環境変数を設定
	putenv("MINISHELL_PIPELINE=1");
	// リダイレクトの設定（パイプラインの後にリダイレクトを適用することで、
	// リダイレクトがパイプラインよりも優先される）
	if (cmd->redirects && !setup_redirection(cmd->redirects))
		exit(1);
	// コマンドの実行
	if (is_builtin(cmd->args[0]))
	{
		status = execute_builtin(cmd->args);
		if (cmd->redirects)
			restore_redirection(cmd->redirects);
		exit(status);
	}
	else
	{
		status = execute_external_command(cmd->args, envp);
		exit(status);
	}
}
#endif

/* コマンドの実行 */
int	execute_command(t_command *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
	{
		ft_printf_fd(2, "Error: Invalid command structure\n");
		return (1);
	}
	ft_printf_fd(2, "DEBUG: Executing command: %s\n", cmd->args[0]);
	if (!cmd->next)
		return (execute_single_command(cmd));
	return (execute_pipeline_command(cmd));
}
