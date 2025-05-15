/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils_quote.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/15 08:48:10 by muiida           ###   ########.fr       */
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

int	get_quoted_string_content(char *input, int *index, char *buffer,
		t_token_type *token_type)
{
	char	quote_char;
	int		word_idx;

	quote_char = input[*index];
	(*index)++;
	word_idx = 0;
	if (quote_char == '\'')
		*token_type = TOKEN_SINGLE_QUOTE;
	else
		*token_type = TOKEN_DOUBLE_QUOTE;
	while (input[*index] && input[*index] != quote_char)
	{
		if (word_idx >= 1024 - 1)
			return (0);
		buffer[word_idx++] = input[*index];
		(*index)++;
	}
	if (input[*index] == quote_char)
	{
		(*index)++;
		buffer[word_idx] = '\0';
		return (expand_and_copy_if_double_quote(buffer, *token_type));
	}
	return (0);
}

/* クォートされた文字列を処理し、トークンリストに追加 */
int	handle_quoted_string(char *input, int *i, t_token **tokens, char *word_buf)

{
	t_token_type	type;
	t_token			*new_token;

	if (!get_quoted_string_content(input, i, word_buf, &type))
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
