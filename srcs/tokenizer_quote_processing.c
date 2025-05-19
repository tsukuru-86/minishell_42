/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_quote_processing.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/05/23 00:07:46 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

// /* クォート文字を処理してトークンタイプを決定 */
static void	set_quote_type_internal(char quote_char, t_token_type *type)
{
	if (quote_char == '\'')
		*type = TOKEN_SINGLE_QUOTE;
	else if (quote_char == '\"')
		*type = TOKEN_DOUBLE_QUOTE;
}

/* クォート内の文字列をバッファにコピー */
static int	copy_quoted_content_internal(char *input, int *i, char *word_buf,
		char quote_char)
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
		t_token_type type, t_command *cmd)
{
	char	*expanded_str;

	if (type == TOKEN_DOUBLE_QUOTE)
	{
		expanded_str = expand_env_vars(word_buf, 1, cmd);
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
int	extract_quoted_string(char *input, int *i, char *word_buf,
		t_token_type *type, t_command *cmd)
{
	char	quote_char;
	int		content_len;

	if (!input || !i || !type)
		return (0);
	quote_char = input[*i];
	if (!is_quote(quote_char))
		return (0);
	set_quote_type_internal(quote_char, type);
	(*i)++;
	content_len = copy_quoted_content_internal(input, i, word_buf, quote_char);
	if (content_len < 0)
		return (0);
	if (input[*i] == quote_char)
	{
		(*i)++;
		if (!expand_and_copy_if_double_quote_internal(word_buf, *type, cmd))
			return (0);
		return (1);
	}
	else
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
		return (0);
	}
}
