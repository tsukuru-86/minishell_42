/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 04:46:54 by muiida            #+#    #+#             */
/*   Updated: 2025/07/01 23:02:32 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/error_messages.h"
#include "minishell.h"

int	process_commands(t_command *cmd)
{
	int	status;

	if (cmd)
	{
		status = execute_commands(cmd);
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

	debug_print("[DEBUG] handle_tokens_and_parse: Start.");
	if (!tokens)
	{
		debug_print("[DEBUG] handle_tokens_and_parse: No tokens, returning 0.");
		return (0);
	}
	debug_print("[DEBUG] handle_tokens_and_parse: Before tokenize.");
	debug_print_tokens(tokens);
	debug_print(
		"[DEBUG] handle_tokens_and_parse: After tokenize (if applicable).");

	if (is_empty_command_tokens(tokens))
	{
		debug_print(
			"[DEBUG] handle_tokens_and_parse: Empty command detected, returning 0.");
		return (0);
	}
	debug_print("[DEBUG] handle_tokens_and_parse: About to call parse_tokens.");
	cmd = parse_tokens(tokens);
	debug_print("[DEBUG] handle_tokens_and_parse: After parse_tokens.");
	if (!cmd)
	{
	debug_print(
		"[DEBUG] handle_tokens_and_parse: Command parsing failed, returning 2.");
	return (2);
}
	print_commands_debug(cmd); /* コマンド構造体をログ出力 */
	debug_print("[DEBUG] handle_tokens_and_parse: About to call process_commands.");
	status = process_commands(cmd);
	debug_print_with_int("[DEBUG] handle_tokens_and_parse: After process_commands, status: ", status);
	debug_print("[DEBUG] handle_tokens_and_parse: Exit.");
	return (status);
}

