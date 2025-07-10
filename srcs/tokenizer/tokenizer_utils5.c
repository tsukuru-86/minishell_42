/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:45:05 by muiida            #+#    #+#             */
/*   Updated: 2025/07/10 14:45:20 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

/* トークンリストの最後のトークンを取得 */
t_token	*get_last_token(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	while (tokens->next)
		tokens = tokens->next;
	return (tokens);
}

/* ホワイトスペース文字をスキップする */
void	skip_whitespace(const char *input, int *i)
{
	while (input[*i] && (input[*i] == ' ' || input[*i] == '\t'
			|| input[*i] == '\n' || input[*i] == '\v' || input[*i] == '\f'
			|| input[*i] == '\r'))
		(*i)++;
}
