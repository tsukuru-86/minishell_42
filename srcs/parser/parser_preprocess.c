/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_preprocess.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 07:43:07 by muiida            #+#    #+#             */
/*   Updated: 2025/06/13 16:02:07 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                        :::      ::::::::   */
/*   parser_preprocess.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 02:16:24 by muiida            #+#    #+#             */
/*   Updated: 2025/06/11 07:45:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	expand_token_content(t_token *current)
{
	char	*expanded;

	if (!current->content)
		return (1);
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
	return (1);
}

int	expand_all_variables(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (1);
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_D_QUOTED_WORD)
		{
			if (!expand_token_content(current))
				return (0);
		}
		current = current->next;
	}
	return (1);
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
