/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_lookahead.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 16:45:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/25 21:36:28 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	collect_command_args(t_command *cmd, t_token *lookahead)
{
	(void)cmd;
	(void)lookahead;
	return (1);
}

int	handle_heredoc_with_lookahead(t_command *cmd, t_token **current_token,
		t_command **head_cmd)
{
	t_token	*lookahead;

	if (!cmd || !current_token || !*current_token || !head_cmd)
		return (0);
	lookahead = (*current_token)->next;
	if (lookahead)
		lookahead = lookahead->next;
	if (!collect_command_args(cmd, lookahead))
	{
		if (*head_cmd)
			free_command(*head_cmd);
		return (0);
	}
	return (handle_heredoc_redirect(cmd, current_token, head_cmd));
}

int	handle_heredoc_redirect(t_command *cmd, t_token **current_token,
		t_command **head_cmd)
{
	t_token	*delimiter_token;

	delimiter_token = (*current_token)->next;
	if (isatty(STDIN_FILENO))
		return (handle_interactive_heredoc(cmd, delimiter_token, head_cmd,
				current_token));
	else
		return (handle_noninteractive_heredoc(cmd, delimiter_token, head_cmd,
				current_token));
}

int	handle_word_type_tokens(t_command **cmd_ptr, t_token **current_token_ptr,
		t_command **head_cmd_ptr)
{
	int	status;

	debug_print("[DEBUG] Handling WORD token");
	status = handle_word_token(*cmd_ptr, current_token_ptr, head_cmd_ptr);
	return (status);
}
