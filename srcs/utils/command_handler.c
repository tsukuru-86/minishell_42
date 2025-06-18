/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 04:46:54 by muiida            #+#    #+#             */
/*   Updated: 2025/06/18 06:32:43 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/error_messages.h"
#include "minishell.h"

int	process_commands(t_command *cmd)
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

	debug_print("[DEBUG] handle_tokens_and_parse called", DEBUG_ENABLED);
	if (!tokens)
		return (0);
	if (is_empty_command_tokens(tokens))
	{
		debug_print("[DEBUG] Empty command detected", DEBUG_ENABLED);
		return (0);
	}
	debug_print("[DEBUG] About to call parse_tokens", DEBUG_ENABLED);
	cmd = parse_tokens(tokens);
	if (!cmd)
		return (2);
	status = process_commands(cmd);
	return (status);
}
