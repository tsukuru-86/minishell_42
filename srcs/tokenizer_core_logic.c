/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_core_logic.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/05/22 00:12:36 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Forward declaration for add_token if it's static in tokenizer.c
 Alternatively, pass tokens list head by reference to modify it.
 For simplicity,
        assuming add_token is available or tokenizer.c handles token list
updates. The original add_token was static in tokenizer.c. To use it here,
        either make it non-static or pass token list management to tokenizer.c
 Let's assume process_token_segment will create and add tokens itself using
create_token and a local add_token logic or by modifying a passed list.
 The original add_token was: static void add_token(t_token **token_list_head,
        t_token *new_token)
 We will replicate a similar logic or assume create_token returns a token
to be added by the caller.*/

/* 通常の単語を処理し、トークンリストに追加 */
static int	handle_word_logic(char *input, int *i, t_token **tokens,
		char *word_buf)
{
	int		word_idx;
	char	*expanded_content;
	t_token	*new_token;

	word_idx = 0;
	while (input[*i] && !is_delimiter(input[*i]) && !is_quote(input[*i])
		&& !is_meta(input[*i]))
	{
		if (word_idx >= MAX_TOKENS - 1)
		{
			ft_putstr_fd((char *)"minishell: token too long\n", 2);
			return (0);
		}
		word_buf[word_idx++] = input[(*i)++];
	}
	word_buf[word_idx] = '\0';
	if (word_idx > 0)
	{
		expanded_content = expand_env_vars(word_buf, 0);
		if (!expanded_content)
			return (0);
		new_token = create_token(expanded_content, TOKEN_WORD);
		free(expanded_content);
		if (!new_token)
			return (0);
		add_token_to_list(tokens, new_token);
	}
	return (1);
}

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
	char    word_buffer[MAX_TOKENS];

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
