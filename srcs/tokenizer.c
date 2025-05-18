/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 04:53:10 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/19 01:29:40 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* トークンをリストの末尾に追加する関数。
   最初のトークンの場合は直接設定し、そうでなければリストを走査して末尾に追加する */
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

t_token	*create_token(char *content, t_token_type type)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->content = ft_strdup(content);
	if (!new_token->content)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

/* ダブルクォート内の文字列の環境変数を展開し、word_bufにコピー */
static int	expand_and_copy_if_double_quote(char *word_buf, t_token_type type)
{
	char	*expanded;

	if (type == TOKEN_DOUBLE_QUOTE)
	{
		// expand_env_vars は minishell.h で宣言されている想定
		expanded = expand_env_vars(word_buf, 1); // 1はダブルクォート内を示す
		if (!expanded)
			return (0);                             // 環境変数展開失敗
		ft_strlcpy(word_buf, expanded, MAX_TOKENS); // MAX_TOKENS はバッファサイズとして仮定
		free(expanded);
	}
	return (1); // 成功またはシングルクォート
}

/* クォート文字を処理してトークンタイプを決定 */
static void	set_quote_type(char quote_char, t_token_type *type, int *i)
{
	if (quote_char == '\'')
		*type = TOKEN_SINGLE_QUOTE;
	else
		*type = TOKEN_DOUBLE_QUOTE;
	(*i)++; // クォート文字をスキップ
}

/* バッファサイズチェックとエラー処理 */
static int	check_buffer_size(int word_idx)
{
	if (word_idx >= 1024 - 1) // バッファ上限チェック
	{
		fprintf(stderr, "Error: String too long\n");
		return (0);
	}
	return (1);
}

/* クォート内の文字列をバッファにコピー */
static int	copy_quoted_content(char *input, int *i, char *word_buf,
		char quote_char)
{
	int	word_idx;

	word_idx = 0;
	while (input[*i] && input[*i] != quote_char)
	{
		if (!check_buffer_size(word_idx))
			return (0);
		word_buf[word_idx++] = input[*i];
		(*i)++;
	}
	word_buf[word_idx] = '\0';
	return (word_idx);
}

/* クォートされた文字列を抽出。クォートが閉じられていない場合などに 0 を返す */
static int	extract_quoted_string(char *input, int *i, char *word_buf,
		t_token_type *type)
{
	char	quote_char;
	int		word_idx;

	quote_char = input[*i];
	set_quote_type(quote_char, type, i);
	word_idx = copy_quoted_content(input, i, word_buf, quote_char);
	if (word_idx == 0 && !check_buffer_size(0))
		return (0);
	if (input[*i] == quote_char)
	{
		(*i)++; // 終端のクォート文字をスキップ
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

	if (extract_quoted_string(input, i, word_buf, &type) == 0)
		return (0); // エラー、呼び出し元で free_tokens
	new_token = create_token(word_buf, type);
	if (!new_token)
		return (0);
	add_token(tokens, new_token);
	return (1); // 成功
}

/* トークン化のメインループの一部を処理 */
static int	process_token_segment(char *input, int *i, t_token **tokens)
{
	char	word_buffer[MAX_TOKENS];

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
	int		i;

	tokens = NULL;
	i = 0;
	if (!input)
		return (NULL);
	while (input[i])
	{
		if (!process_token_segment(input, &i, &tokens))
		{
			free_tokens(tokens);
			return (NULL);
		}
	}
	return (tokens);
}
