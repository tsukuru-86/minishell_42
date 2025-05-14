/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 21:52:17 by muiida            #+#    #+#             */
/*   Updated: 2025/05/15 01:48:13 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	is_delimiter_char(char c)
{
	return (c == '\0' || c == '>' || c == '<' || c == '|' || c == ' ');
}

// static bool	process_token(t_token **tokens, char *token_str,
// 		t_token_type token_type)
// {
// 	t_token	*token;

// 	token = create_token(token_str, token_type);
// 	if (!token)
// 		return (false);
// 	add_token(tokens, token);
// 	return (true);
// }

static bool	handle_delimiter(char *input, int *i)
{
	if (is_delimiter_char(input[*i]))
	{
		(*i)++;
		return (true);
	}
	return (false);
}

static bool	tokenize_input(char *input, t_token **tokens, char *word_buf)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (is_delimiter_char(input[i]))
		{
			handle_delimiter(input, &i);
			continue ;
		}
		if (handle_meta_character(input, &i, tokens) != RET_SUCCESS)
		{
			free_tokens(*tokens);
			return (false);
		}
		else if (handle_word(input, &i, tokens, word_buf) != RET_SUCCESS)
		{
			free_tokens(*tokens);
			return (false);
		}
	}
	return (true);
}

t_token	*tokenize(char *input)
{
	t_token	*tokens;
	char	word_buf[MAX_TOKENS];

	tokens = NULL;
	if (!tokenize_input(input, &tokens, word_buf))
		return (NULL);
	return (tokens);
}
