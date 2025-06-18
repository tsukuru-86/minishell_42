/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:58:32 by muiida       +#+  #+#    #+#             */
/*   Updated: 2025/06/18 13:24:01 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 単一コマンドを実行する関数。リダイレクトを設定し、組み込みコマンドなら直接実行、
外部コマンドならフォークして子プロセスで実行する */
static int	handle_empty_command(t_command *cmd)
{
	int	status;

	if (cmd->redirects)
	{
		status = handle_empty_command_with_redirects();
		restore_redirection(cmd->redirects);
		return (status);
	}
	return (0);
}

static int	execute_single_command(t_command *cmd)
{
	int	status;
	int	idx;

	if (cmd->redirects && !setup_redirection(cmd->redirects))
	{
		set_env_node("?", "1");
		return (1);
	}
	if (!cmd->args || !cmd->args[0])
		return (handle_empty_command(cmd));
	idx = get_builtin_func_idx(cmd->args[0]);
	if (0 <= idx && idx <= 6)
		status = execute_builtin_with_redirect(cmd);
	else
		status = execute_external_command(cmd);
	if (cmd->redirects)
		restore_redirection(cmd->redirects);
	return (status);
}

/* コマンドリストを実行する関数。単一コマンドの場合は直接実行し、
パイプラインの場合はパイプラインのセットアップ、実行、クリーンアップを行う */
int	excute_commands(t_command *cmd)
{
	if (!cmd)
		return (0);
	if (!cmd->next)
	{
		if (cmd->redirects && (!cmd->args || !cmd->args[0]))
		{
			return (execute_single_command(cmd));
		}
		if (!cmd->args || !cmd->args[0])
			return (0);
		return (execute_single_command(cmd));
	}
	return (execute_command_pipeline(cmd));
}
