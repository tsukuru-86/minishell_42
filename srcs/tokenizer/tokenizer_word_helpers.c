/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_word_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:17:28 by muiida            #+#    #+#             */
/*   Updated: 2025/07/10 14:38:13 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int		extract_quoted_content(const char *input, int *i, char *buf,
			int *buf_len);
void	extract_word_content(const char *input, int *i, char *buf,
			int *buf_len);

int	handle_heredoc_word_token(t_tokenizer_stat *stat, const char *input)
{
	t_token	*new_token;

	new_token = create_heredoc_delimiter_token(input, &stat->i_input);
	if (!new_token)
		return (0);
	add_token_to_list(&stat->tokens, new_token);
	return (1);
}

int	handle_quoted_word_token(t_tokenizer_stat *stat, const char *input, int *i)
{
	char			buf[1024];
	int				buf_len;
	t_token			*new_token;
	t_token_type	token_type;

	buf_len = 0;
	token_type = extract_quoted_content(input, i, buf, &buf_len);
	buf[buf_len] = '\0';
	new_token = create_expanded_token(buf, token_type);
	add_token_to_list(&stat->tokens, new_token);
	return (1);
}

int	handle_plain_word_token(t_tokenizer_stat *stat, const char *input, int *i)
{
	char	buf[1024];
	int		buf_len;
	t_token	*new_token;

	buf_len = 0;
	extract_word_content(input, i, buf, &buf_len);
	buf[buf_len] = '\0';
	new_token = create_expanded_token(buf, TOKEN_WORD);
	add_token_to_list(&stat->tokens, new_token);
	return (1);
}
