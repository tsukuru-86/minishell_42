/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_preprocess_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 02:39:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/13 18:41:57 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	should_remove_token(t_token *token)
{
	if (token->type == TOKEN_SINGLE_QUOTE || token->type == TOKEN_DOUBLE_QUOTE)
		return (1);
	return (0);
}

static void	remove_and_free_token(t_token *current, t_token **prev,
		t_token **new_head)
{
	if (*prev)
		(*prev)->next = current->next;
	else
		*new_head = current->next;
	if (current->content)
		free(current->content);
	free(current);
}

static void	process_quote_token(t_token *current, t_token **prev)
{
	if (current->type == TOKEN_S_QUOTED_WORD
		|| current->type == TOKEN_D_QUOTED_WORD)
	{
		current->type = TOKEN_WORD;
	}
	*prev = current;
}

t_token	*remove_quote_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;
	t_token	*prev;
	t_token	*new_head;

	current = tokens;
	prev = NULL;
	new_head = tokens;
	while (current)
	{
		next = current->next;
		if (should_remove_token(current))
			remove_and_free_token(current, &prev, &new_head);
		else
			process_quote_token(current, &prev);
		current = next;
	}
	return (new_head);
}
