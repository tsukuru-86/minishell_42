/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 04:46:54 by muiida            #+#    #+#             */
/*   Updated: 2025/06/12 17:35:49 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/error_messages.h"
#include "minishell.h"
#include "utils/debug_utils.h"

int	process_commands(t_command *cmd, int debug)
{
	int	status;

	if (cmd)
	{
		if (cmd->args)
			print_args_debug(cmd->args, debug);
		status = excute_commands(cmd);
		set_exit_status(cmd, status);
		free_command(cmd);
		return (status);
	}
	ft_putstr_fd((char *)ERR_SYNTAX_ERROR, 2);
	return (2);
}

int	handle_tokens_and_parse(t_token *tokens, int debug)
{
	t_command	*cmd;
	int			status;

	if (!tokens)
		return (2);
	cmd = parse_tokens(tokens);
	if (!cmd)
		return (2);
	status = process_commands(cmd, debug);
	return (status);
}
