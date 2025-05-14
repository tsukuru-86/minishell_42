/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_handlers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/15 02:28:11 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* ダブルクォート内の文字列の環境変数を展開し、word_bufにコピー */
static int	expand_and_copy_if_double_quote(char *word_buf, t_token_type type)
{
	char	*expanded;

	if (type == TOKEN_DOUBLE_QUOTE)
	{
		expanded = expand_env_vars(word_buf, 1);
		if (!expanded)
			return (0);
		ft_strlcpy(word_buf, expanded, MAX_TOKENS);
		free(expanded);
	}
	return (1);
}

/* クォートされた文字列を抽出 */
static int	extract_quoted_string(char *input, int *i, char *word_buf,
		t_token_type *type)
{
	char	quote_char;
	int		word_idx;

	quote_char = input[*i];
	(*i)++;
	word_idx = 0;
	if (quote_char == '\'')
		*type = TOKEN_SINGLE_QUOTE;
	else
		*type = TOKEN_DOUBLE_QUOTE;
	while (input[*i] && input[*i] != quote_char)
	{
		if (word_idx >= 1024 - 1)
			return (0);
		word_buf[word_idx++] = input[*i];
		(*i)++;
	}
	if (input[*i] == quote_char)
	{
		(*i)++;
		word_buf[word_idx] = '\0';
		return (expand_and_copy_if_double_quote(word_buf, *type));
	}
	return (0);
}

/* 通常の単語を処理し、トークンリストに追加 */
int	handle_word(char *input, int *i, t_token **tokens, char *word_buf)
{
	int		word_idx;
	char	*expanded;
	t_token	*new_token;

	word_idx = 0;
	while (input[*i] && !is_delimiter(input[*i]) && !is_quote(input[*i])
		&& !is_meta(input[*i]))
		word_buf[word_idx++] = input[(*i)++];
	word_buf[word_idx] = '\0';
	if (word_idx > 0)
	{
		expanded = expand_env_vars(word_buf, 0);
		if (!expanded)
			return (0);
		new_token = create_token(expanded, TOKEN_WORD);
		free(expanded);
		if (!new_token)
			return (0);
		if (!add_token(tokens, new_token->content, new_token->type))
		{
			free(new_token);
			return (0);
		}
		free(new_token);
	}
	return (1);
}

/* メタ文字を処理し、トークンリストに追加 */
int	handle_meta_character(char *input, int *i, t_token **tokens)
{
	t_token	*new_token;

	new_token = create_meta_token(input, i);
	if (!new_token)
		return (0);
	if (!add_token(tokens, new_token->content, new_token->type))
	{
		free(new_token);
		return (0);
	}
	free(new_token);
	return (1);
}

/* クォートされた文字列を処理し、トークンリストに追加 */
int	handle_quoted_string(char *input, int *i, t_token **tokens, char *word_buf)
{
	t_token_type	type;
	t_token			*new_token;

	if (!extract_quoted_string(input, i, word_buf, &type))
		return (0);
	new_token = create_token(word_buf, type);
	if (!new_token)
		return (0);
	if (!add_token(tokens, new_token->content, new_token->type))
	{
		free(new_token);
		return (0);
	}
	free(new_token);
	return (1);
}
