/* ************************************************************************** */
/*         	while (current)
	{
		next = current->next;
		if (current->type == TOKEN_SPACE)
		{
			if (prev)
				prev->next = current->next;
			else
				new_head = current->next;
			if (current->content)
				free(current->content);
			free(current);
		}
		else
			prev = current;
		current = next;
	}                                                 */
/*                                                        :::      ::::::::   */
/*   parser_preprocess.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 02:16:24 by muiida            #+#    #+#             */
/*   Updated: 2025/06/05 02:16:24 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	expand_all_variables(t_token *tokens)
{
	t_token	*current;
	char	*expanded;

	if (!tokens)
		return (1);
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_D_QUOTED_WORD)
		{
			if (!current->content)
			{
				current = current->next;
				continue ;
			}
			expanded = expand_env_vars(current->content,
					current->type == TOKEN_D_QUOTED_WORD);
			if (!expanded)
			{
				ft_putstr_fd("minishell: expansion failed\n", 2);
				return (0);
			}
			if (current->content)
				free(current->content);
			current->content = expanded;
		}
		current = current->next;
	}
	return (1);
}

static int	is_space_token(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_SPACE);
}

static void	remove_token_node(
	t_token **current, t_token **prev, t_token **new_head)
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
	{
		if (is_space_token(current))
			remove_token_node(&current, &prev, &new_head);
		else
		{
			prev = current;
			current = current->next;
		}
	}
	return (new_head);
}

t_token	*preprocess_tokens(t_token *tokens)
{
	t_token	*processed_tokens;

	if (!tokens)
		return (NULL);
	if (!expand_all_variables(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	processed_tokens = remove_quote_tokens(tokens);
	if (!processed_tokens)
		return (NULL);
	if (processed_tokens)
		processed_tokens = remove_empty_tokens(processed_tokens);
	if (processed_tokens)
		processed_tokens = merge_adjacent_non_meta_tokens(processed_tokens);
	if (processed_tokens)
		processed_tokens = remove_space_tokens(processed_tokens);
	return (processed_tokens);
}
