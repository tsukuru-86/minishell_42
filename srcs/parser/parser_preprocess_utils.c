/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_preprocess_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 02:39:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/05 02:41:35 by muiida           ###   ########.fr       */
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

static void	process_quote(t_token *current, t_token **prev, t_token **head)
{
	if (current->type == TOKEN_SINGLE_QUOTE
		|| current->type == TOKEN_DOUBLE_QUOTE)
		remove_quote_node(current, prev, head);
	else
	{
		if (current->type == TOKEN_S_QUOTED_WORD
			|| current->type == TOKEN_D_QUOTED_WORD)
			current->type = TOKEN_WORD;
		*prev = current;
	}
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
		process_quote(current, &prev, &new_head);
		current = next;
	}
	return (new_head);
}

static char	*merge_word_chain(t_token *current)
{
	char	*merged;
	char	*temp;
	t_token	*next;

	merged = ft_strdup(current->content);
	if (!merged)
		return (NULL);
	next = current->next;
	while (next && next->type == TOKEN_WORD)
	{
		temp = ft_strjoin(merged, next->content);
		if (!temp)
			return (free(merged), NULL);
		free(merged);
		merged = temp;
		current->next = next->next;
		free(next->content);
		free(next);
		next = current->next;
	}
	return (merged);
}

t_token	*merge_adjacent_non_meta_tokens(t_token *tokens)
{
	t_token	*current;
	char	*merged;

	current = tokens;
	while (current)
	{
		if (current->type != TOKEN_WORD || !current->next
			|| current->next->type != TOKEN_WORD)
		{
			current = current->next;
			continue ;
		}
		merged = merge_word_chain(current);
		if (!merged)
			return (NULL);
		free(current->content);
		current->content = merged;
		current = current->next;
	}
	return (tokens);
}
