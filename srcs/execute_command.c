/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 05:02:31 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/18 03:23:12 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* コマンドの実行 */
int	execute_command(t_command *cmd, t_minishell *shell)
{
	setup_parent_signals();
	if (!cmd || !cmd->args || !cmd->args[0])
	{
		ft_printf_fd(2, "Error: Invalid command structure\n");
		shell->exit_status = 1;
		return (1);
	}
	ft_printf_fd(2, "DEBUG: Executing command: %s\n", cmd->args[0]);
	if (!cmd->next)
		return (execute_single_command(cmd, shell));
	init_signal_handlers();
	return (execute_pipeline_command(cmd, shell));
}

static int	handle_builtin_command(t_command *cmd, t_minishell *shell)
{
	int	status;

	init_signal_handlers();
	status = execute_builtin(cmd);
	shell->exit_status = status;
	if (cmd->redirects)
		restore_redirection(cmd->redirects);
	return (status);
}

static int	execute_external_in_child(t_command *cmd)
{
	char	*cmd_path;
	char	**args;

	cmd_path = find_command(cmd->args[0]);
	if (!cmd_path)
	{
		ft_printf_fd(2, "minishell: %s: command not found\n", cmd->args[0]);
		exit(127);
	}
	args = prepare_command(cmd->args, cmd->redirects ? 1 : 0);
	if (!args)
	{
		free(cmd_path);
		exit(1);
	}
	launch_child(cmd_path, cmd->args);
	free_array_upto_index(args, 0);
	return (0);
}

static int	wait_for_child(pid_t pid, t_minishell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status = 128 + WTERMSIG(status);
	shell->exit_status = status;
	return (status);
}

int	execute_single_command(t_command *cmd, t_minishell *shell)
{
	int		status;
	pid_t	pid;

	if (cmd->redirects && !setup_redirection(cmd->redirects))
	{
		shell->exit_status = 1;
		return (1);
	}
	if (is_builtin(cmd->args[0]))
		return (handle_builtin_command(cmd, shell));
	// 外部コマンド実行前に親プロセスのシグナルハンドラを設定
	setup_parent_signals();
	pid = fork();
	if (pid == -1)
	{
		shell->exit_status = 1;
		return (1);
	}
	if (pid == 0)
	{
		setup_child_signals(); // 子プロセスのシグナルハンドラを設定
		execute_external_in_child(cmd);
	}
	status = wait_for_child(pid, shell);
	// 外部コマンド終了後、通常のシグナルハンドラに戻す
	init_signal_handlers();
	if (cmd->redirects)
		restore_redirection(cmd->redirects);
	return (status);
}
