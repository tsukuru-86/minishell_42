/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_preprocess_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 02:39:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/10 05:37:32 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static void	remove_quote_node(t_token *current, t_token **prev, t_token **head)
{
	if (*prev)
		(*prev)->next = current->next;
	else
		*head = current->next;
	free(current->content);
	free(current);
}

static int	should_remove_token(t_token *token)
{
	if (token->type == TOKEN_SINGLE_QUOTE || token->type == TOKEN_DOUBLE_QUOTE)
		return (1);
	if ((token->type == TOKEN_S_QUOTED_WORD
			|| token->type == TOKEN_D_QUOTED_WORD) && token->content
		&& ft_strlen(token->content) == 0)
		return (1);
	return (0);
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
			remove_quote_node(current, &prev, &new_head);
		else
		{
			if (current->type == TOKEN_S_QUOTED_WORD
				|| current->type == TOKEN_D_QUOTED_WORD)
				current->type = TOKEN_WORD;
			prev = current;
		}
		current = next;
	}
	return (new_head);
}
