/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_meta_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 06:57:30 by muiida            #+#    #+#             */
/*   Updated: 2025/07/13 06:58:53 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

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

// heredocデリミタの引用符付きコピー
int	fill_heredoc_quoted(const char *input, int *i, char *word_buffer)
{
	char	quote;
	int		word_i;

	word_i = 0;
	quote = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote)
	{
		word_buffer[word_i] = input[*i];
		word_i++;
		(*i)++;
	}
	if (input[*i] == quote)
		(*i)++;
	return (word_i);
}

// heredocデリミタの非引用符コピー
int	fill_heredoc_unquoted(const char *input, int *i, char *word_buffer)
{
	int	word_i;

	word_i = 0;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
		&& input[*i] != '<' && input[*i] != '>' && !is_quote(input[*i]))
	{
		word_buffer[word_i] = input[*i];
		word_i++;
		(*i)++;
	}
	return (word_i);
}
