/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_meta_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/05/20 22:24:50 by muiida            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* メタ文字のトークンタイプを判定 */
static t_token_type	get_meta_type(char *input, int *i)
{
	if (input[*i] == '|')
	{
		(*i)++;
		return (TOKEN_PIPE);
	}
	else if (input[*i] == '<')
	{
		(*i)++;
		// Note: Original code did not check for '<<' (here document) here.
		// Assuming simple '<' and '>' '>>' based on original get_meta_type.
		return (TOKEN_REDIR_IN);
	}
	else if (input[*i] == '>')
	{
		(*i)++;
		if (input[*i] == '>')
		{
			(*i)++;
			return (TOKEN_REDIR_APPEND);
		}
		return (TOKEN_REDIR_OUT);
	}
	return (TOKEN_WORD); // Should not happen if called correctly
}

/* メタ文字トークンを作成 */
t_token	*create_meta_token(char *input, int *i)
{
	t_token_type type;
	char meta_char_val;
	char meta_str[3]; // Max 2 chars + null terminator (e.g., ">>")
	int original_i;

	original_i = *i;
	meta_char_val = input[*i];      // Save the first char for the string
	type = get_meta_type(input, i); // i is advanced by get_meta_type

	if (type == TOKEN_PIPE || type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT)
	{
		meta_str[0] = meta_char_val;
		meta_str[1] = '\0';
	}
	else if (type == TOKEN_REDIR_APPEND)
	{
		meta_str[0] = meta_char_val; // Should be '>'
		meta_str[1] = '>';
		meta_str[2] = '\0';
	}
	else
	{
		// Should not reach here if input[*i] was a meta character
		// Restore i if no valid meta token was formed.
		*i = original_i;
		return (NULL); // Or handle error appropriately
	}
	return (create_token(meta_str, type));
}