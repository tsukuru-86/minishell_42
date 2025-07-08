/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_core_logic.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/07/09 02:21:14 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

t_token	*create_token(char *content, t_token_type type)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
	{
		perror("minishell: malloc failed for token");
		return (NULL);
	}
	if (content)
	{
		new_token->content = ft_strdup(content);
		if (!new_token->content)
		{
			perror("minishell: malloc failed for token content");
			free(new_token);
			return (NULL);
		}
	}
	else
		new_token->content = NULL;
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

/* クォートされた文字列を処理し、トークンリストに追加 */
/* トークン化のメインループの一部を処理 */
