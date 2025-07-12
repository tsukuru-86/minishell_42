/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_preprocess_space.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 07:05:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/11 07:05:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	is_space_token(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_SPACE);
}

static void	remove_token_node(t_token **current, t_token **prev,
		t_token **new_head)
{
	t_token	*next;

	next = (*current)->next;
	if (*prev)
		(*prev)->next = next;
	else
		*new_head = next;
	if ((*current)->content)
		free((*current)->content);
	free(*current);
	*current = next;
}

static void	process_space_token(t_token **current, t_token **prev,
		t_token **new_head)
{
	if (is_space_token(*current))
		remove_token_node(current, prev, new_head);
	else
	{
		*prev = *current;
		*current = (*current)->next;
	}
}

t_token	*remove_space_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*prev;
	t_token	*new_head;

	if (!tokens)
		return (NULL);
	current = tokens;
	prev = NULL;
	new_head = tokens;
	while (current)
		process_space_token(&current, &prev, &new_head);
	return (new_head);
}
