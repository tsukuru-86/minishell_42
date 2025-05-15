/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 02:23:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/15 04:41:27 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*add_token(t_token **head, const char *content, t_token_type type)
{
	t_token	*new_token;
	t_token	*temp;

	new_token = create_token((char *)content, type);
	if (!new_token)
	{
		perror("minishell: failed to create token in add_token");
		return (NULL);
	}
	if (*head == NULL)
	{
		*head = new_token;
		return (new_token);
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_token;
	return (new_token);
}

bool	process_token_in_parse_loop(t_command **cmd_ptr,
		t_token **current_token_ptr, char **accumulated_arg_ptr)
{
	t_token_type	type;
	static int		cmd_idx;

	if (!current_token_ptr || !(*current_token_ptr))
		return (true);
	type = (*current_token_ptr)->type;
	if (type == TOKEN_WORD || type == TOKEN_SINGLE_QUOTE
		|| type == TOKEN_DOUBLE_QUOTE || type == TOKEN_QUOTED_WORD
		|| type == TOKEN_ENV_VAR)
		return (process_word_token(current_token_ptr, accumulated_arg_ptr));
	else if (type == TOKEN_SPACE)
		return (process_space_token(*cmd_ptr, current_token_ptr,
				accumulated_arg_ptr));
	else if (type == TOKEN_PIPE)
		return (handle_pipe_token(current_token_ptr, &cmd_ptr,
				&cmd_idx) == RET_SUCCESS);
	else if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC)
		return (handle_redirect_token(current_token_ptr,
				*cmd_ptr) == RET_SUCCESS);
	return (handle_error_token(current_token_ptr, accumulated_arg_ptr));
}

bool	process_tokens(t_command **cmd, t_token *tokens)
{
	t_token	*current;
	char	*accumulated_arg;

	current = tokens;
	accumulated_arg = NULL;
	ft_printf_fd(2, "DEBUG: START PROCESSING TOKENS\n");
	while (current)
	{
		ft_printf_fd(2, "DEBUG: CURRENT TOKEN: %s (type: %d)\n",
			current->content, current->type);
		if (!process_token_in_parse_loop(cmd, &current, &accumulated_arg))
		{
			if (accumulated_arg)
				free(accumulated_arg);
			return (false);
		}
	}
	ft_printf_fd(2, "DEBUG: TOKEN PROCESSING COMPLETE\n");
	return (handle_final_arg(cmd, accumulated_arg));
}
