/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_quote_processing.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/05/20 22:24:50 by muiida            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* バッファサイズチェックとエラー処理 */
static int	check_buffer_size_internal(int word_idx, int buffer_limit)
{
	if (word_idx >= buffer_limit - 1) // buffer_limit is typically MAX_TOKENS
	{
		ft_putstr_fd("minishell: token too long during quote processing\n", 2);
		return (0); // Error, buffer overflow would occur
	}
	return (1); // OK
}

/* クォート文字を処理してトークンタイプを決定 */
static void	set_quote_type_internal(char quote_char, t_token_type *type)
{
	if (quote_char == '\'')
		*type = TOKEN_SINGLE_QUOTE;
	else if (quote_char == '\"')
		*type = TOKEN_DOUBLE_QUOTE;
	// Else, type remains unchanged or an error state if not a quote
}

/* クォート内の文字列をバッファにコピー */
static int	copy_quoted_content_internal(char *input, int *i, char *word_buf,
		char quote_char, int buffer_limit)
{
	int	word_idx;

	word_idx = 0;
	while (input[*i] && input[*i] != quote_char)
	{
		if (!check_buffer_size_internal(word_idx, buffer_limit))
			return (-1); // Error signaled by -1, or use 0 and check errno
		word_buf[word_idx++] = input[(*i)++];
	}
	word_buf[word_idx] = '\0'; // Null-terminate the copied content
	return (word_idx);
	// Return length of copied content, or -1 on error
}

/* ダブルクォート内の文字列の環境変数を展開し、word_bufにコピー */
static int	expand_and_copy_if_double_quote_internal(char *word_buf,
		t_token_type type, int buffer_limit)
{
	char	*expanded_str;

	if (type == TOKEN_DOUBLE_QUOTE)
	{
		// expand_env_vars is assumed to be available (e.g. from env_expand.c)
		expanded_str = expand_env_vars(word_buf, 1); // 1 indicates in_dquote
		if (!expanded_str)
		{
			ft_putstr_fd("minishell: environment variable expansion failed in double quote\n",
				2);
			return (0); // Expansion failure
		}
		// Ensure expanded string fits into word_buf
		if (ft_strlen(expanded_str) >= (size_t)buffer_limit)
		{
			ft_putstr_fd("minishell: expanded string too long for buffer\n", 2);
			free(expanded_str);
			return (0); // Failure, too long
		}
		ft_strlcpy(word_buf, expanded_str, buffer_limit);
		free(expanded_str);
	}
	return (1); // Success, or no expansion needed (single quote)
}

/* クォートされた文字列を抽出。クォートが閉じられていない場合などに 0 を返す */
// word_buf is the output buffer, type is an output parameter for token type.
// Returns 1 on success, 0 on failure. i is advanced.
int	extract_quoted_string(char *input, int *i, char *word_buf,
		t_token_type *type)
{
	char quote_char;
	int content_len;

	if (!input || !i || !word_buf || !type)
		return (0); // Invalid arguments
	quote_char = input[*i];
	if (!is_quote(quote_char))
		return (0); // Not a quote character at input[*i]

	set_quote_type_internal(quote_char, type);
	(*i)++; // Consume the opening quote

	// Copy content within quotes into word_buf
	// MAX_TOKENS is the assumed buffer size for word_buf
	content_len = copy_quoted_content_internal(input, i, word_buf, quote_char,
			MAX_TOKENS);
	if (content_len < 0) // Error during copy (e.g., too long)
		return (0);

	if (input[*i] == quote_char)
	{
		(*i)++; // Consume the closing quote
		// If double quote, expand variables in word_buf
		if (!expand_and_copy_if_double_quote_internal(word_buf, *type,
				MAX_TOKENS))
			return (0); // Expansion or copy failed
		return (1);     // Success
	}
	else
	{
		// Unclosed quote
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
		return (0); // Failure
	}
}