/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_token_handlers2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:45:28 by muiida            #+#    #+#             */
/*   Updated: 2025/07/10 15:03:04 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

/* メタ文字セグメントを処理してトークンを作成し、リストに追加 */
int	handle_meta_token_creation(t_tokenizer_stat *stat, const char *input)
{
	t_token	*new_token;

	new_token = create_meta_token(input, &stat->i_input);
	if (!new_token)
		return (0);
	add_token_to_list(&stat->tokens, new_token);
	return (1);
}

/* ホワイトスペース文字をスキップする */
/* 実装はtokenizer_utils5.cに移動 */
