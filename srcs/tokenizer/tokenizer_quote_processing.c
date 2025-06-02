/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_quote_processing.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/06/03 04:40:29 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../error/error_messages.h"
#include "minishell.h"
#include "tokenizer.h"

/* バッファサイズチェックとエラー処理 */
static int	check_buffer_size_internal(int word_idx)
{
	if (word_idx >= MAX_TOKENS - 1)
	{
		ft_putstr_fd("minishell: token too long during quote processing\n", 2);
		return (0);
	}
	return (1);
}

static void	set_quote_type_internal(char quote_char, t_token_type *type)
{
	if (quote_char == '\'')
		*type = TOKEN_S_QUOTED_WORD;
	else if (quote_char == '\"')
		*type = TOKEN_D_QUOTED_WORD;
}

/* クォート内の文字列をバッファにコピー */
static int	copy_quoted_content_internal(const char *input, int *i,
		char *word_buf, char quote_char)
{
	int	word_idx;

	word_idx = 0;
	while (input[*i] && input[*i] != quote_char)
	{
		if (!check_buffer_size_internal(word_idx))
			return (-1);
		word_buf[word_idx++] = input[(*i)++];
	}
	word_buf[word_idx] = '\0';
	return (word_idx);
}

/* ダブルクォート内の文字列の環境変数を展開し、word_bufにコピー */

static int	expand_and_copy_if_double_quote_internal(char *word_buf,
		t_token_type type)
{
	char	*expanded_str;

	if (type == TOKEN_D_QUOTED_WORD)
	{
		expanded_str = expand_env_vars(word_buf, 1);
		if (!expanded_str)
		{
			ft_putstr_fd("minishell: environment", 2);
			ft_putstr_fd(" variable expansion failed in double quote\n", 2);
			return (0);
		}
		if (ft_strlen(expanded_str) >= (size_t)MAX_TOKENS)
		{
			ft_putstr_fd("minishell: expanded string too long for buffer\n", 2);
			free(expanded_str);
			return (0);
		}
		ft_strlcpy(word_buf, expanded_str, MAX_TOKENS);
		free(expanded_str);
	}
	return (1);
}

/* クォートされた文字列を抽出。クォートが閉じられていない場合などに 0 を返す */
int	extract_quoted_string(t_tokenizer_stat *stat, const char *input,
		char *word_buf)
{
	char	quote_char;

	if (!input || !stat || !is_quote(input[stat->i_input]))
		return (0);
	quote_char = input[stat->i_input];
	set_quote_type_internal(quote_char, &stat->quote_type);
	stat->i_input++;
	if (copy_quoted_content_internal(input, &stat->i_input, word_buf,
			quote_char) < 0)
		return (0);
	if (input[stat->i_input] != quote_char)
	{
		ft_putstr_fd((char *)ERR_UNCLOSED_QUOTE, 2);
		return (0);
	}
	stat->i_input++;
	if (!expand_and_copy_if_double_quote_internal(word_buf, stat->quote_type))
		return (0);
	return (1);
}
