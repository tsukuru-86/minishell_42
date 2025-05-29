/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 05:37:47 by muiida            #+#    #+#             */
/*   Updated: 2025/05/30 08:48:50 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

/* トークナイザの状態を初期化。cmdがNULLの場合、新しいコマンド構造体を割り当てる */
int	init_tokenizer_stat(t_tokenizer_stat *stat, t_command *cmd)
{
	stat->tokens = NULL;
	stat->i_input = 0;
	stat->needs_cmd_free = false;
	if (!cmd)
	{
		stat->cmd = (t_command *)malloc(sizeof(t_command));
		if (!stat->cmd)
			return (0);
		ft_memset(stat->cmd, 0, sizeof(t_command));
		stat->cmd->last_status = 0;
		stat->needs_cmd_free = true;
	}
	else
		stat->cmd = cmd;
	return (1);
}

/* 単語セグメントを抽出し、展開し、トークンを作成してリストに追加 */
int	handle_word_token_creation(t_tokenizer_stat *stat, const char *input)
{
	t_token	*new_token;
	int		raw_word_len;
	int		expansion_status;

	raw_word_len = extract_raw_word(input, &stat->i_input, stat->word_buffer);
	if (raw_word_len == 0)
		return (1);
	new_token = create_expanded_word_token(stat->word_buffer,
			&expansion_status);
	if (expansion_status == 0)
		return (0);
	if (expansion_status == 1)
		return (1);
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
