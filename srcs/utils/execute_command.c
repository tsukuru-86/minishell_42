/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:58:32 by muiida       +#+  #+#    #+#             */
/*   Updated: 2025/06/21 13:57:24 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../redirect/redirect.h"

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
	if (cmd->args && cmd->args[0] && cmd->args[0][0] == '\0')
	{
		ft_putstr_fd("minishell: : command not found\n", 2);
		set_env_node("?", "127");
		return (127);
	}
	return (0);
}

static int	execute_single_command(t_command *cmd)
{
	int		status;
	int		idx;
	t_env	*status_node;

	if (cmd->redirects && !setup_redirection(cmd->redirects))
	{
		status_node = get_env_node("?");
		if (status_node && ft_strcmp(status_node->value, "0") == 0)
			return (0);
		set_env_node("?", "1");
		return (1);
	}
	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
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
int	execute_commands(t_command *cmd)
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
