/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_meta_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:45:35 by muiida            #+#    #+#             */
/*   Updated: 2025/07/10 14:55:58 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

/* パイプ判定 */
t_token_type	check_pipe_token(const char *input, int *i)
{
	if (input[*i] == '|')
	{
		(*i)++;
		return (TOKEN_PIPE);
	}
	return (TOKEN_WORD);
}

/* <, << 判定 */
t_token_type	check_less_than_token(const char *input, int *i)
{
	if (input[*i] == '<')
	{
		(*i)++;
		if (input[*i] == '<')
		{
			(*i)++;
			return (TOKEN_HEREDOC);
		}
		return (TOKEN_REDIR_IN);
	}
	return (TOKEN_WORD);
}

/* heredocデリミタ用のトークンを作成（クォート対応） */
int	fill_heredoc_quoted(const char *input, int *i, char *word_buffer)
{
	int		word_i;
	char	quote_char;

	word_i = 0;
	quote_char = input[*i];
	word_buffer[word_i++] = input[(*i)++];
	while (input[*i] && input[*i] != quote_char)
	{
		if (word_i < 1024 - 1)
			word_buffer[word_i++] = input[*i];
		(*i)++;
	}
	if (input[*i] == quote_char)
		word_buffer[word_i++] = input[(*i)++];
	word_buffer[word_i] = '\0';
	return (word_i);
}

int	fill_heredoc_unquoted(const char *input, int *i, char *word_buffer)
{
	int	word_i;

	word_i = 0;
	while (input[*i] && !is_delimiter(input[*i]) && !is_meta(input[*i])
		&& input[*i] != '\n')
	{
		if (word_i < 1024 - 1)
			word_buffer[word_i++] = input[*i];
		(*i)++;
	}
	word_buffer[word_i] = '\0';
	return (word_i);
}

/* >, >> 判定 */
t_token_type	check_greater_than_token(const char *input, int *i)
{
	if (input[*i] == '>')
	{
		(*i)++;
		if (input[*i] == '>')
		{
			(*i)++;
			return (TOKEN_REDIR_APPEND);
		}
		return (TOKEN_REDIR_OUT);
	}
	return (TOKEN_WORD);
}
