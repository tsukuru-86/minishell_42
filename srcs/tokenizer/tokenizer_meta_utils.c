/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_meta_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/07/03 04:33:33 by muiida           ###   ########.fr       */
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

static t_token_type	check_less_than_token(const char *input, int *i)
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

static t_token_type	check_greater_than_token(const char *input, int *i)
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

/* 直前のトークンがheredocかどうかを確認 */
int	should_mark_as_heredoc_delimiter(t_token *tokens)
{
	t_token	*current;
	t_token	*last_non_space;

	if (!tokens)
		return (0);
	current = tokens;
	last_non_space = NULL;
	while (current)
	{
		if (current->type != TOKEN_SPACE)
			last_non_space = current;
		current = current->next;
	}
	if (last_non_space && last_non_space->type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

/* heredocデリミタ用のトークンを作成（クォート対応） */
t_token	*create_heredoc_delimiter_token(const char *input, int *i)
{
	char	word_buffer[1024];
	int		word_i;
	char	quote_char;

	word_i = 0;
	if (is_quote(input[*i]))
	{
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
	}
	else
	{
		while (input[*i] && !is_delimiter(input[*i]) && !is_meta(input[*i])
			&& input[*i] != '\n')
		{
			if (word_i < 1024 - 1)
				word_buffer[word_i++] = input[*i];
			(*i)++;
		}
	}
	word_buffer[word_i] = '\0';
	debug_print_with_str("DEBUG: created heredoc delimiter token", word_buffer);
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
