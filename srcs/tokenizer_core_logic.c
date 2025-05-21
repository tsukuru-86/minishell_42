/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_core_logic.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/05/22 00:34:50 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdbool.h>

/* Forward declaration for add_token if it's static in tokenizer.c
 Alternatively, pass tokens list head by reference to modify it.
 For simplicity,
		assuming add_token is available or tokenizer.c handles token list
updates. The original add_token was static in tokenizer.c. To use it here,
		either make it non-static or pass token list management to tokenizer.c
 Let's assume process_token_segment will create and add tokens itself using
create_token and a local add_token logic or by modifying a passed list.
 The original add_token was: static void add_token(t_token **token_list_head,
		t_token *new_token)
 We will replicate a similar logic or assume create_token returns a token
to be added by the caller.*/

/* 入力から区切り文字、クォート、メタ文字以外の文字を収集する。成功時は収集した文字数を返す
 handle_word_logic からのみ呼ばれる */
static int	collect_plain_word_segment(char *input, int *i, char *word_buf)
{
	int	word_idx;

	word_idx = 0;
	while (input[*i] && !is_delimiter(input[*i]) && !is_quote(input[*i])
		&& !is_meta(input[*i]))
	{
		if (word_idx >= MAX_TOKENS - 1)
		{
			ft_putstr_fd((char *)"minishell: token too long\n", 2);
			return (-1);
		}
		word_buf[word_idx++] = input[(*i)++];
	}
	word_buf[word_idx] = '\0';
	return (word_idx);
}

/* 新しいヘルパー関数: 収集された単語を環境変数展開し、トークンを作成してリストに追加
 handle_word_logic からのみ呼ばれるため static にする */
static bool	create_and_add_word_token(char *word_buf, t_token **tokens)
{
	char	*expanded_content;
	t_token	*new_token;

	expanded_content = expand_env_vars(word_buf, 0);
	if (!expanded_content)
	{
		ft_putstr_fd("minishell: failed to expand env vars for word\n", 2);
		return (false);
	}
	new_token = create_token(expanded_content, TOKEN_WORD);
	free(expanded_content);
	if (!new_token)
	{
		ft_putstr_fd("minishell: failed to create token for word\n", 2);
		return (false);
	}
	add_token_to_list(tokens, new_token);
	return (true);
}

/* 通常の単語を処理し、トークンリストに追加 */
static int	handle_word_logic(char *input, int *i, t_token **tokens,
		char *word_buf)
{
	int	word_idx;

	word_idx = collect_plain_word_segment(input, i, word_buf);
	if (word_idx < 0)
		return (0);
	if (word_idx > 0)
	{
		if (!create_and_add_word_token(word_buf, tokens))
			return (0);
	}
	return (1);
}

/* メタ文字を処理し、トークンリストに追加 */
static int	handle_meta_char_logic(char *input, int *i, t_token **tokens)
{
	t_token	*new_token;

	new_token = create_meta_token(input, i);
	if (!new_token)
	{
		return (0);
	}
	add_token_to_list(tokens, new_token);
	return (1);
}

/* クォートされた文字列を処理し、トークンリストに追加 */
static int	handle_quoted_string_logic(char *input, int *i, t_token **tokens,
		char *word_buf)
{
	t_token_type	type;
	t_token			*new_token;

	if (!extract_quoted_string(input, i, word_buf, &type))
	{
		return (0);
	}
	new_token = create_token(word_buf, type);
	if (!new_token)
		return (0);
	add_token_to_list(tokens, new_token);
	return (1);
}

/* トークン化のメインループの一部を処理 */

int	process_token_segment(char *input, int *i, t_token **tokens)
{
	char	word_buffer[MAX_TOKENS];

	if (is_delimiter(input[*i]))
	{
		(*i)++;
		return (1);
	}
	else if (is_quote(input[*i]))
	{
		return (handle_quoted_string_logic(input, i, tokens, word_buffer));
	}
	else if (is_meta(input[*i]))
	{
		return (handle_meta_char_logic(input, i, tokens));
	}
	else
	{
		return (handle_word_logic(input, i, tokens, word_buffer));
	}
}
