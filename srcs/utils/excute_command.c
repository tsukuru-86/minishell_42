/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:58:32 by muiida       +#+  #+#    #+#             */
/*   Updated: 2025/07/15 18:16:48 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect/redirect.h"
#include "minishell.h"

/* Execute a single command. Set up redirection, execute directly if builtin,
or fork and execute in a child process if external command. */
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
		perror(cmd->redirects->file);
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

/* Execute a command list. If a single command, execute directly.
If a pipeline, set up, execute, and clean up the pipeline. */
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
