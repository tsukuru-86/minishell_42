/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 04:53:10 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/13 02:28:09 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* トークンをリストに追加*/
static void	add_token(t_token **token, t_token *new_token)
{
	t_token	*current;

	if (!(*token))
	{
		*token = new_token;
		return ;
	}
	current = *token;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

/* ダブルクォート内の文字列の環境変数を展開し、word_bufにコピー */
static int	expand_and_copy_if_double_quote(char *word_buf,
		t_token_type type)
{
	char	*expanded;

	if (type == TOKEN_DOUBLE_QUOTE)
	{
		// expand_env_vars は minishell.h で宣言されている想定
		expanded = expand_env_vars(word_buf, 1); // 1はダブルクォート内を示す
		if (!expanded)
			return (0); // 環境変数展開失敗
		// word_buf のサイズは呼び出し元(extract_quoted_string)で十分確保されている前提
		ft_strlcpy(word_buf, expanded, MAX_TOKENS); // MAX_TOKENS はバッファサイズとして仮定
		free(expanded);
	}
	return (1); // 成功またはシングルクォート
}

/* クォートされた文字列を抽出 */
static int	extract_quoted_string(char *input, int *i, char *word_buf,
		t_token_type *type)
{
	char	quote_char;
	int		word_idx;

	quote_char = input[*i];
	(*i)++; // クォート文字をスキップ
	word_idx = 0;
	if (quote_char == '\'')
		*type = TOKEN_SINGLE_QUOTE;
	else
		*type = TOKEN_DOUBLE_QUOTE;
	while (input[*i] && input[*i] != quote_char)
	{
		if (word_idx >= 1024 - 1) // バッファ上限チェック
			return (0);           // エラー: 文字列が長すぎる
		word_buf[word_idx++] = input[*i];
		(*i)++;
	}
	if (input[*i] == quote_char)
	{
		(*i)++; // 終端のクォート文字をスキップ
		word_buf[word_idx] = '\0';
		return (expand_and_copy_if_double_quote(word_buf, *type));
	}
	return (0); // クォートが閉じられていない
}

/* 通常の単語を処理し、トークンリストに追加 */
static int	handle_word(char *input, int *i, t_token **tokens, char *word_buf)
{
	int		word_idx;
	char	*expanded;
	t_token	*new_token;

	word_idx = 0;
	// 通常の単語を抽出
	while (input[*i] && !is_delimiter(input[*i]) && !is_quote(input[*i])
		&& !is_meta(input[*i]))
		word_buf[word_idx++] = input[(*i)++];
	word_buf[word_idx] = '\0';
	if (word_idx > 0)
	{
		// 環境変数の展開
		expanded = expand_env_vars(word_buf, 0);
		if (!expanded)
			return (0); // 環境変数展開失敗
		new_token = create_token(expanded, TOKEN_WORD);
		free(expanded);
		if (!new_token)
			return (0); // トークン作成失敗
		add_token(tokens, new_token);
	}
	return (1); // 成功 (単語がなくてもエラーではない)
}


/* メタ文字を処理し、トークンリストに追加 */
static int	handle_meta_character(char *input, int *i, t_token **tokens)
{
	t_token	*new_token;

	new_token = create_meta_token(input, i);
	if (!new_token)
		return (0); // メモリ確保失敗または不正なメタ文字
	add_token(tokens, new_token);
	return (1); // 成功
}


/* クォートされた文字列を処理し、トークンリストに追加 */
static int	handle_quoted_string(char *input, int *i, t_token **tokens,
		char *word_buf)
{
	t_token_type	type;
	t_token			*new_token;

	// extract_quoted_string はクォートが閉じられていない場合などに 0 を返す
	if (!extract_quoted_string(input, i, word_buf, &type))
		return (0); // エラー、呼び出し元で free_tokens
	new_token = create_token(word_buf, type);
	if (!new_token)
		return (0); // メモリ確保失敗
	add_token(tokens, new_token);
	return (1); // 成功
}

/* トークン化のメインループの一部を処理 */
static int	process_token_segment(char *input, int *i, t_token **tokens,
		char *word_buffer)
{
	if (is_delimiter(input[*i]))
	{
		(*i)++;
		return (1); // Skip delimiter
	}
	else if (is_quote(input[*i]))
	{
		return (handle_quoted_string(input, i, tokens, word_buffer));
	}
	else if (is_meta(input[*i]))
	{
		return (handle_meta_character(input, i, tokens));
	}
	else // Regular word
	{
		return (handle_word(input, i, tokens, word_buffer));
	}
}

/* 入力文字列をトークンに分割 */
t_token	*tokenize(char *input)
{
	t_token	*tokens;
	char	word_buffer[MAX_TOKENS]; // Buffer for word/quoted string content
	int		i;

	tokens = NULL;
	i = 0;
	if (!input)
		return (NULL);
	while (input[i])
	{
		if (!process_token_segment(input, &i, &tokens, word_buffer))
		{
			free_tokens(tokens); // Free partially created tokens on error
			return (NULL);     // Indicate tokenization error
		}
	}
	return (tokens);
}
