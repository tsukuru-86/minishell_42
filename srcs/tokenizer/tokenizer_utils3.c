/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:40:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/10 15:05:49 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
int	extract_quoted_content(const char *input, int *i, char *buf, int *buf_len)
{
	int		j;
	char	quote;

	j = 0;
	quote = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote)
	{
		buf[j++] = input[*i];
		(*i)++;
	}
	if (input[*i] == quote)
		(*i)++;
	*buf_len = j;
	return (quote == '\'');
}

/* 通常の単語を抽出する */
void	extract_word_content(const char *input, int *i, char *buf, int *buf_len)
{
	int	j;

	j = 0;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
		&& input[*i] != '\'' && input[*i] != '\"'
		&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
	{
		buf[j++] = input[*i];
		(*i)++;
	}
	*buf_len = j;
}

/* 単語セグメントを抽出し、展開し、トークンを作成してリストに追加 */
int				handle_heredoc_word_token(t_tokenizer_stat *stat,
					const char *input);

int				handle_quoted_word_token(t_tokenizer_stat *stat,
					const char *input, int *i);
int				handle_plain_word_token(t_tokenizer_stat *stat,
					const char *input, int *i);

int	handle_word_token_creation(t_tokenizer_stat *stat, const char *input)
{
	int	i;
	int	ret;

	i = stat->i_input;
	if (should_mark_as_heredoc_delimiter(stat->tokens))
		return (handle_heredoc_word_token(stat, input));
	if (is_quote(input[i]))
	{
		ret = handle_quoted_word_token(stat, input, &i);
		stat->i_input = i;
		return (ret);
	}
	else
	{
		ret = handle_plain_word_token(stat, input, &i);
		stat->i_input = i;
		return (ret);
	}
}
