/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_remove.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 20:30:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/03 04:48:30 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
#include "parser.h"

static t_token	*remove_empty_token(t_token *tokens, t_token *curr,
		t_token *prev)
{
	t_token	*next;

	next = curr->next;
	if (prev)
		prev->next = next;
	else
		tokens = next;
	if (curr->content)
		free(curr->content);
	free(curr);
	return (tokens);
}
t_token	*remove_empty_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*prev;
	t_token	*next;

	current = tokens;
	prev = NULL;
	while (current)
	{
		next = current->next;
		if (current->type == TOKEN_EMPTY_VAR)
		{
			tokens = remove_empty_token(tokens, current, prev);
			current = next;
			continue ;
		}
		prev = current;
		current = next;
	}
	return (tokens);
}
t_token	*remove_quote_tokens(t_token *tokens)
{
	t_token	*current;
	char	*unquoted_content;

	current = tokens;
	while (current)
	{
		if ((current->type == TOKEN_S_QUOTED_WORD
				|| current->type == TOKEN_D_QUOTED_WORD)
			&& current->type != TOKEN_HEREDOC_DELIMITER)
		{
			unquoted_content = ft_strdup(current->content);
			if (!unquoted_content)
			{
				return (NULL);
			}
			free(current->content);
			current->content = unquoted_content;
			current->type = TOKEN_WORD;
		}
		current = current->next;
	}
	return (tokens);
}
