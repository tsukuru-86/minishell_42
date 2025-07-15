/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_handlers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 10:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/22 10:00:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/error_messages.h"
#include "minishell.h"
#include "parser.h"

/*
** @brief Handle word tokens. Merge adjacent non-space tokens.
** @param cmd Current command structure
** @param current_token Pointer to the current token
** @param head_cmd Pointer to the head of the command list
** @return 1 on success, 0 on failure
*/
int	handle_word_token(t_command *cmd, t_token **current_token,
		t_command **head_cmd)
{
	char	*merged_content;

	if (!cmd || !current_token || !*current_token || !head_cmd)
		return (0);
	merged_content = merge_adjacent_tokens(current_token);
	if (!merged_content)
	{
		if (*head_cmd)
			free_command(*head_cmd);
		return (0);
	}
	if (!add_argument(cmd, merged_content))
	{
		free(merged_content);
		if (*head_cmd)
			free_command(*head_cmd);
		return (0);
	}
	free(merged_content);
	return (1);
}

/*
** @brief Handle pipe tokens.
** @param cmd Pointer to the current command structure
** @param current_token Pointer to the current token
** @param head_cmd Pointer to the head of the command list
** @return 1 on success, 0 on failure
*/
int	handle_pipe_token(t_command **cmd, t_token **current_token,
		t_command **head_cmd)
{
	if (!cmd || !*cmd || !current_token || !*current_token || !head_cmd)
		return (0);
	(*cmd)->next = create_command();
	if (!(*cmd)->next)
	{
		if (*head_cmd)
			free_command(*head_cmd);
		*head_cmd = NULL;
		return (0);
	}
	*cmd = (*cmd)->next;
	*current_token = (*current_token)->next;
	if (!*current_token)
	{
		ft_printf_fd(2, ERR_UNEXP_TOKEN, "|");
		if (*head_cmd)
			free_command(*head_cmd);
		*head_cmd = NULL;
		return (0);
	}
	return (1);
}

/*
** @brief Handle token processing in the parse_tokens loop.
** @param cmd_ptr Pointer to the current command structure
** @param current_token_ptr Pointer to the current token
** @param head_cmd_ptr Pointer to the head of the command list
** @return 1 on success, 0 on failure
*/
int	process_token_in_parse_loop(t_command **cmd_ptr,
		t_token **current_token_ptr, t_command **head_cmd_ptr)
{
	t_token_type	type;

	if (!cmd_ptr || !*cmd_ptr || !current_token_ptr || !*current_token_ptr
		|| !head_cmd_ptr)
		return (0);
	type = (*current_token_ptr)->type;
	if (type == TOKEN_WORD || type == TOKEN_S_QUOTED_WORD
		|| type == TOKEN_D_QUOTED_WORD)
		return (handle_word_token(*cmd_ptr, current_token_ptr,
				head_cmd_ptr));
	else if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC)
		return (handle_redirect_type_tokens(cmd_ptr, current_token_ptr,
				head_cmd_ptr));
	else if (type == TOKEN_PIPE)
		return (handle_pipe_token(cmd_ptr, current_token_ptr, head_cmd_ptr));
	else
		return (handle_other_tokens(current_token_ptr, type));
}
