/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_core_logic.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/05/22 01:16:33 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* メタ文字を処理し、トークンリストに追加 */
static int	handle_meta_char_logic(char *input, int *i, t_token **tokens)
{
	t_token	*new_token;

	new_token = create_meta_token(input, i);
	if (!new_token)
	{
		return (0);
	}
	add_token_to_list(tokens, new_token);
	return (1);
}

/* クォートされた文字列を処理し、トークンリストに追加 */
static int	handle_quoted_string_logic(char *input, int *i, t_token **tokens,
		char *word_buf)
{
	t_token_type	type;
	t_token			*new_token;

	if (!extract_quoted_string(input, i, word_buf, &type))
	{
		return (0);
	}
	new_token = create_token(word_buf, type);
	if (!new_token)
		return (0);
	add_token_to_list(tokens, new_token);
	return (1);
}

/* トークン化のメインループの一部を処理 */

int	process_token_segment(char *input, int *i, t_token **tokens)
{
	char	word_buffer[MAX_TOKENS];

	if (is_delimiter(input[*i]))
	{
		(*i)++;
		return (1);
	}
	else if (is_quote(input[*i]))
	{
		return (handle_quoted_string_logic(input, i, tokens, word_buffer));
	}
	else if (is_meta(input[*i]))
	{
		return (handle_meta_char_logic(input, i, tokens));
	}
	else
	{
		return (handle_word_logic(input, i, tokens, word_buffer));
	}
}
