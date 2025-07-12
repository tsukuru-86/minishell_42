/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_meta_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/07/13 06:56:50 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

static t_token_type	check_pipe_token(const char *input, int *i)
{
	if (input[*i] == '|')
	{
		(*i)++;
		return (TOKEN_PIPE);
	}
	return (TOKEN_WORD);
}

t_token_type	check_less_than_token(const char *input, int *i);

t_token_type	check_greater_than_token(const char *input, int *i);

/* メタ文字のトークンタイプを判定。Returns TOKEN_WORD if no meta char matched */
t_token_type	get_meta_type(const char *input, int *i)
{
	t_token_type	type;

	type = check_pipe_token(input, i);
	if (type != TOKEN_WORD)
		return (type);
	type = check_less_than_token(input, i);
	if (type != TOKEN_WORD)
		return (type);
	type = check_greater_than_token(input, i);
	return (type);
}

t_token	*create_heredoc_delimiter_token(const char *input, int *i)
{
	char	word_buffer[1024];
	int		word_i;

	if (is_quote(input[*i]))
		word_i = fill_heredoc_quoted(input, i, word_buffer);
	else
		word_i = fill_heredoc_unquoted(input, i, word_buffer);
	word_buffer[word_i] = '\0';
	return (safe_create_token(word_buffer, TOKEN_HEREDOC_DELIMITER));
}

/* メタ文字トークンを作成 */
t_token	*create_meta_token(const char *input, int *i)
{
	t_token_type	type;
	char			meta_str[3];
	int				len;

	len = 0;
	meta_str[len++] = input[*i];
	type = get_meta_type(input, i);
	if (type == TOKEN_REDIR_APPEND)
		meta_str[len++] = '>';
	else if (type == TOKEN_HEREDOC)
		meta_str[len++] = '<';
	meta_str[len] = '\0';
	return (safe_create_token(meta_str, type));
}
