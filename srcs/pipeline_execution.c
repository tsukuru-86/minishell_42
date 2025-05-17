/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:50:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/18 03:32:45 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* リダイレクト設定と検証 */
static void	setup_command_redirection(t_command *current)
{
	if (current->redirects && !setup_redirection(current->redirects))
	{
		ft_printf_fd(2, "minishell: redirection failed\n");
		exit(EXIT_FAILURE);
	}
}

/* コマンド引数の検証 */
static void	validate_command_args(t_command *current)
{
	if (!current->args || !current->args[0])
	{
		ft_printf_fd(2, "minishell: empty command\n");
		exit(EXIT_FAILURE);
	}
}

/* パイプラインのI/Oリダイレクト処理 */
static void	pipeline_redirect_io(t_command *current)
{
	if (current->pipe.read_fd != -1)
	{
		if (dup2(current->pipe.read_fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
	}
	if (current->pipe.write_fd != -1)
	{
		if (dup2(current->pipe.write_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
	}
}

/* パイプライン内のコマンド実行ロジック */
static void	pipeline_execute_command_logic(t_command *current)
{
	int	status;

	setup_command_redirection(current);
	validate_command_args(current);
	if (is_builtin(current->args[0]))
	{
		status = execute_builtin(current);
		if (current->redirects)
			restore_redirection(current->redirects);
		exit(status);
	}
	else
	{
		execute_external_command(current);
		exit(EXIT_FAILURE);
	}
}

/* パイプライン内のコマンド実行 */
void	execute_command_in_pipeline(t_command *cmd, t_command *current)
{
	if (!cmd || !current)
		return ;
	if (current->pipe.read_fd != -1)
		close(current->pipe.read_fd);
	if (current->pipe.write_fd != -1)
		close(current->pipe.write_fd);
	if (current->redirects)
		setup_redirection(current->redirects);
	{
		setup_child_signals();
		set_env_var(g_env(), "MINISHELL_PIPELINE", "1");
		pipeline_redirect_io(current);
		pipeline_close_pipes(cmd);
		pipeline_execute_command_logic(current);
	}
}
