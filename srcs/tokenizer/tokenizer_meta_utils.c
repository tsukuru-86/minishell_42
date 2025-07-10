/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_meta_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/07/10 14:56:33 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

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

t_token	*create_heredoc_delimiter_token(const char *input, int *i)
{
	char	word_buffer[1024];
	int		word_i;

	if (is_quote(input[*i]))
		word_i = fill_heredoc_quoted(input, i, word_buffer);
	else
		word_i = fill_heredoc_unquoted(input, i, word_buffer);
	word_buffer[word_i] = '\0';
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
