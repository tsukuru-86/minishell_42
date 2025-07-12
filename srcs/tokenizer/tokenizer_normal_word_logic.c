/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_normal_word_logic.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 00:51:16 by muiida            #+#    #+#             */
/*   Updated: 2025/06/20 22:13:30 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

/* 入力から区切り文字、クォート、メタ文字以外の文字を収集する。成功時は収集した文字数を返す
 handle_word_logic からのみ呼ばれる */
static int	collect_plain_word_segment(const char *input, int *i,
		char *word_buf)
{
	int	word_idx;

	word_idx = 0;
	while (input[*i] && !is_delimiter(input[*i]) && !is_quote(input[*i])
		&& !is_meta(input[*i]) && input[*i] != '\n')
	{
		if (word_idx >= MAX_TOKENS - 1)
		{
			ft_putstr_fd((char *)"minishell: token too long\n", 2);
			return (-1);
		}
		word_buf[word_idx++] = input[(*i)++];
	}
	word_buf[word_idx] = '\0';
	return (word_idx);
}

static bool	create_and_add_word_token(char *word_buf, t_token **tokens)
{
	char	*expanded_content;
	t_token	*new_token;

	expanded_content = expand_env_vars(word_buf, 0);
	if (!expanded_content)
	{
		ft_putstr_fd((char *)"minishell: failed to \n", 2);
		ft_putstr_fd((char *)" expand env vars for word\n", 2);
		return (false);
	}
	new_token = safe_create_token(expanded_content, TOKEN_WORD);
	free(expanded_content);
	if (!new_token)
	{
		ft_putstr_fd((char *)"minishell: failed to create token for word\n", 2);
		return (false);
	}
	add_token_to_list(tokens, new_token);
	return (true);
}

/* 通常の単語を処理し、トークンリストに追加 */
int	handle_word_logic(char *input, int *i, t_token **tokens, char *word_buf)
{
	int	word_idx;

	word_idx = collect_plain_word_segment(input, i, word_buf);
	if (word_idx < 0)
		return (0);
	if (word_idx > 0)
	{
		if (!create_and_add_word_token(word_buf, tokens))
			return (0);
	}
	return (1);
}
