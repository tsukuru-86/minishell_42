/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 21:50:45 by muiida            #+#    #+#             */
/*   Updated: 2025/06/20 08:47:29 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "parser.h"

int	is_word_type_token(t_token_type type)
{
	return (type == TOKEN_WORD || type == TOKEN_S_QUOTED_WORD
		|| type == TOKEN_D_QUOTED_WORD);
}

char	*merge_adjacent_tokens(t_token **current_token_ptr)
{
	char	*content;
	t_token	*current;

	current = *current_token_ptr;
	if (!current || !is_word_type_token(current->type))
		return (NULL);
	content = ft_strdup(current->content);
	*current_token_ptr = current->next;
	return (content);
}

void	free_redirects(t_redirect *redirects)
{
	t_redirect	*current;
	t_redirect	*next;

	current = redirects;
	while (current)
	{
		next = current->next;
		free(current->file);
		free(current);
		current = next;
	}
}
