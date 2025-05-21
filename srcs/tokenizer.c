/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 04:53:10 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/22 00:47:35 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* トークンをリストの末尾に追加する関数。 */

void	add_token_to_list(t_token **token_list_head, t_token *new_token)
{
	t_token	*current;

	if (!token_list_head || !new_token)
		return ;
	if (!(*token_list_head))
	{
		*token_list_head = new_token;
		return ;
	}
	current = *token_list_head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

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

/* 入力文字列をトークンに分割 */
t_token	*tokenize(char *input)
{
	t_token	*tokens_head;
	int		i;

	tokens_head = NULL;
	i = 0;
	if (!input)
		return (NULL);
	while (input[i])
	{
		if (!process_token_segment(input, &i, &tokens_head))
		{
			free_tokens(tokens_head);
			return (NULL);
		}
	}
	return (tokens_head);
}
