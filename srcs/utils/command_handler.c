/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 04:46:54 by muiida            #+#    #+#             */
/*   Updated: 2025/07/24 01:47:15 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/error_messages.h"
#include "minishell.h"
#include "debug/debug.h"

static int	process_commands(t_command *cmd)
{
	int	status;

	if (cmd)
	{
		status = excute_commands(cmd);
		set_exit_status(cmd, status);
		free_command(cmd);
		return (status);
	}
	ft_putstr_fd((char *)ERR_SYNTAX_ERROR, 2);
	return (2);
}

int	is_empty_command_tokens(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (1);
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_D_QUOTED_WORD
			|| current->type == TOKEN_S_QUOTED_WORD)
		{
			if (current->content)
				return (0);
		}
		if (current->type != TOKEN_WORD && current->type != TOKEN_D_QUOTED_WORD
			&& current->type != TOKEN_S_QUOTED_WORD
			&& current->type != TOKEN_NEWLINE)
		{
			return (0);
		}
		current = current->next;
	}
	return (1);
}

int	handle_tokens_and_parse(t_token *tokens)
{
	t_command	*cmd;
	int			status;

	dbg_printf("handle_tokens_and_parse called");
	if (!tokens)
		return (0);
	if (is_empty_command_tokens(tokens))
	{
		dbg_printf("Empty command detected");
		free_tokens(tokens);
		return (0);
	}
	dbg_printf("About to call parse_tokens");
	cmd = parse_tokens(tokens);
	if (!cmd)
		return (2);
	status = process_commands(cmd);
	return (status);
}
