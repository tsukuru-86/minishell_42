/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 20:46:04 by muiida            #+#    #+#             */
/*   Updated: 2025/06/03 04:40:29 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

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

/* 空白文字をトークンとして処理し、リストに追加 */
int	handle_space_token_creation(t_tokenizer_stat *stat, const char *input)
{
	char		space_buf[1024];
	int			buf_len;
	int			i;
	t_token		*new_token;

	i = stat->i_input;
	buf_len = 0;
	while (input[i] && is_delimiter(input[i]))
	{
		if (buf_len < 1023)
			space_buf[buf_len++] = input[i];
		i++;
	}
	space_buf[buf_len] = '\0';
	stat->i_input = i;
	new_token = create_token(space_buf, TOKEN_SPACE);
	if (!new_token)
		return (0);
	add_token_to_list(&stat->tokens, new_token);
	return (1);
}
