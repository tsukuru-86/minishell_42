/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:40:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/03 04:34:21 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

/* 環境変数を展開してトークンを作成 */
static t_token	*create_expanded_var_token(char *buf, t_token_type type)
{
	char	*expanded;
	t_token	*new_token;
	int		in_quotes;

	if (type == TOKEN_D_QUOTED_WORD)
		in_quotes = 1;
	else
		in_quotes = 0;
	expanded = expand_env_vars(buf, in_quotes);
	if (!expanded)
		return (NULL);
	new_token = safe_create_token(expanded, type);
	free(expanded);
	return (new_token);
}

/* トークンを作成し、必要に応じて環境変数を展開する */
t_token	*create_expanded_token(char *buf, t_token_type token_type)
{
	if (token_type == TOKEN_D_QUOTED_WORD || token_type == TOKEN_WORD)
		return (create_expanded_var_token(buf, token_type));
	else
		return (safe_create_token(buf, token_type));
}

/* クォートされた文字列を抽出する */
static int	extract_quoted_content(const char *input, int *i, char *buf,
		int *buf_len)
{
	char	quote_c;
	int		ret;

	quote_c = input[*i];
	(*i)++;
	*buf_len = 0;
	while (input[*i] && input[*i] != quote_c)
	{
		if (*buf_len < 1023)
			buf[(*buf_len)++] = input[*i];
		(*i)++;
	}
	if (input[*i] == quote_c)
		(*i)++;
	if (quote_c == '\'')
		ret = TOKEN_S_QUOTED_WORD;
	else
		ret = TOKEN_D_QUOTED_WORD;
	buf[*buf_len] = '\0';
	return (ret);
}

/* 通常の単語を抽出する */
static void	extract_word_content(const char *input, int *i, char *buf,
		int *buf_len)
{
	if (input[*i] == '$' && (input[*i + 1] == '"' || input[*i + 1] == '\''))
		(*i)++;
	while (input[*i] && !is_delimiter(input[*i]) && !is_quote(input[*i])
		&& !is_meta(input[*i]) && input[*i] != '\n')
	{
		if (*buf_len < 1023)
			buf[(*buf_len)++] = input[*i];
		(*i)++;
	}
}

/* 単語セグメントを抽出し、展開し、トークンを作成してリストに追加 */
int	handle_word_token_creation(t_tokenizer_stat *stat, const char *input)
{
	char			buf[1024];
	int				buf_len;
	t_token			*new_token;
	t_token_type	token_type;
	int				i;

	i = stat->i_input;
	buf_len = 0;
	if (should_mark_as_heredoc_delimiter(stat->tokens))
	{
		debug_print("DEBUG: creating heredoc delimiter token via create_heredoc_delimiter_token");
		new_token = create_heredoc_delimiter_token(input, &stat->i_input);
		if (!new_token)
			return (0);
		add_token_to_list(&stat->tokens, new_token);
		return (1);
	}
	if (is_quote(input[i]))
		token_type = extract_quoted_content(input, &i, buf, &buf_len);
	else
	{
		extract_word_content(input, &i, buf, &buf_len);
		token_type = TOKEN_WORD;
	}
	buf[buf_len] = '\0';
	stat->i_input = i;
	new_token = create_expanded_token(buf, token_type);
	add_token_to_list(&stat->tokens, new_token);
	return (1);
}
