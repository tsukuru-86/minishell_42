/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils_meta.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/14 06:18:16 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief メタ文字の種類を判断します
 */
t_token_type	get_meta_type(char *input, int *i)
{
	if (input[*i] == '|')
		return (TOKEN_PIPE);
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			(*i)++;
			return (TOKEN_HEREDOC);
		}
		return (TOKEN_REDIR_IN);
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			(*i)++;
			return (TOKEN_REDIR_APPEND);
		}
		return (TOKEN_REDIR_OUT);
	}
	return (TOKEN_WORD); // ここには通常到達しない
}

/**
 * @brief メタ文字トークンを作成します
 */
t_token	*create_meta_token(char *input, int *i)
{
	t_token			*new_token;
	t_token_type	type;
	char			content[3];
	int				j;

	j = 0;
	type = get_meta_type(input, i);
	if (type == TOKEN_HEREDOC || type == TOKEN_REDIR_APPEND)
	{
		content[j++] = input[*i - 1];
		content[j++] = input[(*i)++];
	}
	else
		content[j++] = input[(*i)++];
	content[j] = '\0';
	new_token = create_token(content, type);
	return (new_token);
}

// その他のメタ文字処理関連関数