/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 05:02:31 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/20 22:33:29 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 単一コマンドを実行する関数。リダイレクトを設定し、組み込みコマンドなら直接実行、
外部コマンドならフォークして子プロセスで実行する */
static int	execute_single_command(t_command *cmd)
{
	int	status;

	if (cmd->redirects && !setup_redirection(cmd->redirects))
	{
		ft_putstr_fd((char *)"minishell: redirection error\n", 2);
		return (1);
	}
	if (is_builtin(cmd->args[0]))
		status = execute_builtin_with_redirect(cmd);
	else
	{
		status = execute_external_with_fork(cmd);
		if (cmd->redirects)
			restore_redirection(cmd->redirects);
	}
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

/* パイプラインのクリーンアップを行う関数 */
static void	cleanup_pipeline_commands(t_command *cmd)
{
	t_command	*current;

	current = cmd;
	while (current)
	{
		cleanup_pipeline(current);
		current = current->next;
	}
}

/* パイプラインコマンドを実行する関数 */
static int	execute_command_pipeline(t_command *cmd)
{
	int	pipeline_result;
	int	status;

	pipeline_result = setup_pipeline(cmd);
	if (pipeline_result == 0)
	{
		ft_putstr_fd((char *)"minishell: pipeline setup error\n", 2);
		return (1);
	}
	status = wait_pipeline(cmd);
	cleanup_pipeline_commands(cmd);
	return (status);
}

/* コマンドリストを実行する関数。単一コマンドの場合は直接実行し、
パイプラインの場合はパイプラインのセットアップ、実行、クリーンアップを行う */
int	excute_commands(t_command *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (!cmd->next)
	{
		if (!cmd || !cmd->args || !cmd->args[0])
			return (0);
		return (execute_single_command(cmd));
	}
	return (execute_command_pipeline(cmd));
}
