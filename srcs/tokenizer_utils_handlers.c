/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils_handlers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/15 08:54:24 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 単語をバッファにコピー */
static int	copy_word_to_buffer(char *input, int *i, char *word_buf)
{
	int	word_idx;

	word_idx = 0;
	while (input[*i] && !is_delimiter(input[*i]) && !is_quote(input[*i])
		&& !is_meta(input[*i]))
		word_buf[word_idx++] = input[(*i)++];
	word_buf[word_idx] = '\0';
	return (word_idx);
}

/* 通常の単語を処理し、トークンリストに追加 */
int	handle_word(char *input, int *i, t_token **tokens, char *word_buf)
{
	int		word_idx;
	char	*expanded;
	t_token	*new_token;

	word_idx = copy_word_to_buffer(input, i, word_buf);
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

char	*extract_quoted_string(char *input, int *i, char quote,
		t_token_type *type)
{
	char	*content;
	int		start;
	int		len;
	char	*expanded;

	start = *i + 1;
	len = 0;
	while (input[start + len] && input[start + len] != quote)
		len++;
	if (!input[start + len])
		return (NULL);
	content = ft_substr(input, start, len);
	if (!content)
		return (NULL);
	if (quote == '"')
	{
		*type = TOKEN_DOUBLE_QUOTE;
		expanded = expand_env_vars(content, 1);
		free(content);
		content = expanded;
	}
	else
		*type = TOKEN_SINGLE_QUOTE;
	*i = start + len + 1;
	return (content);
}
