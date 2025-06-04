/* ************************************************************************** */
/*                                                                            */
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

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_D_QUOTED_WORD)
		{
			expanded = expand_env_vars(current->content,
					current->type == TOKEN_D_QUOTED_WORD);
			if (!expanded)
			{
				ft_putstr_fd("minishell: expansion failed\n", 2);
				return (0);
			}
			free(current->content);
			current->content = expanded;
		}
		current = current->next;
	}
	return (1);
}

t_token	*preprocess_tokens(t_token *tokens)
{
	t_token	*processed_tokens;

	if (!tokens || !expand_all_variables(tokens))
		return (NULL);
	processed_tokens = remove_quote_tokens(tokens);
	if (!processed_tokens)
		return (NULL);
	// processed_tokens = merge_adjacent_non_meta_tokens(processed_tokens);
	// if (!processed_tokens)
	//	return (NULL);
	processed_tokens = remove_space_tokens(processed_tokens);
	return (processed_tokens);
}
