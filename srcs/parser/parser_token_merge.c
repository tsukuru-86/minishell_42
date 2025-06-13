/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_merge.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 20:30:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/13 20:32:32 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static char	*join_and_cleanup_token(char *merged, t_token *next)
{
	char	*temp;

	if (!next->content)
	{
		next->content = ft_strdup("");
		if (!next->content)
			return (NULL);
	}
	temp = ft_strjoin(merged, next->content);
	if (!temp)
		return (NULL);
	return (temp);
}

static int	is_mergeable_token(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_WORD);
}

static char	*merge_word_chain(t_token *current)
{
	char	*merged;
	char	*temp;
	t_token	*next;

	if (!current || !current->content)
		return (NULL);
	merged = ft_strdup(current->content);
	if (!merged)
		return (NULL);
	next = current->next;
	while (next && is_mergeable_token(next))
	{
		temp = join_and_cleanup_token(merged, next);
		if (!temp)
			return (free(merged), NULL);
		free(merged);
		merged = temp;
		current->next = next->next;
		if (next->content)
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
		if (!is_mergeable_token(current) || !current->next
			|| !is_mergeable_token(current->next))
		{
			current = current->next;
			continue ;
		}
		if (!current->content)
			current->content = ft_strdup("");
		merged = merge_word_chain(current);
		if (!merged)
		{
			current = current->next;
			continue ;
		}
		free(current->content);
		current->content = merged;
		current = current->next;
	}
	return (tokens);
}
