/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:42:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/13 18:43:09 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

/* トークンをリストの末尾に追加 */
void	add_token_to_list(t_token **token_list_head, t_token *new_token)
{
	t_token	*current;

	if (!new_token)
		return ;
	if (!*token_list_head)
	{
		*token_list_head = new_token;
		return ;
	}
	current = *token_list_head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

/* トークンリストの最後のトークンを取得 */
t_token	*get_last_token(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	while (tokens->next)
		tokens = tokens->next;
	return (tokens);
}

/* トークナイザーを終了処理 */
void	finalize_tokenizer(t_tokenizer_stat *vars)
{
	if (!vars)
		return ;
}

/* トークナイザーのクリーンアップとNULL返却 */
t_token	*cleanup_and_return_null(t_tokenizer_stat *vars, char *input)
{
	if (vars && vars->tokens)
	{
		free_tokens(vars->tokens);
		vars->tokens = NULL;
	}
	if (input)
		free(input);
	return (NULL);
}

/* 空白トークンの作成処理 */
int	handle_space_token_creation(t_tokenizer_stat *vars, const char *input)
{
	t_token	*space_token;

	(void)input;
	space_token = create_token(" ", TOKEN_SPACE);
	if (!space_token)
		return (0);
	add_token_to_list(&vars->tokens, space_token);
	vars->i_input++;
	return (1);
}
