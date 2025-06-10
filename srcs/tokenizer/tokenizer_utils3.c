/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 20:46:04 by muiida            #+#    #+#             */
/*   Updated: 2025/06/03 21:49:13 by muiida           ###   ########.fr       */
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
	char	space_buf[1024];
	int		buf_len;
	int		i;
	t_token	*new_token;

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

/* トークンリストと内部で割り当てられたコマンド構造体を解放し、NULLを返す */
t_token	*cleanup_and_return_null(t_tokenizer_stat *stat, char *input)
{
	if (!handle_word_token_creation(stat, input))
		free_tokens(stat->tokens);
	stat->tokens = NULL;
	if (stat->needs_cmd_free && stat->cmd)
	{
		free(stat->cmd);
		stat->cmd = NULL;
	}
	return (NULL);
}

/* トークナイズ処理の最後に呼び出され、内部で割り当てられたコマンド構造体を解放 */
void	finalize_tokenizer(t_tokenizer_stat *stat)
{
	if (stat->needs_cmd_free && stat->cmd)
	{
		free(stat->cmd);
		stat->cmd = NULL;
	}
}

/* トークンリストの最後のトークンを取得 */
t_token	*get_last_token(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (NULL);
	current = tokens;
	while (current->next)
		current = current->next;
	return (current);
}
