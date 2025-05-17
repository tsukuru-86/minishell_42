/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 21:52:17 by muiida            #+#    #+#             */
/*   Updated: 2025/05/16 15:07:14 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	process_token(char *input, int *i, t_token **tokens, char *word_buf,
		t_minishell *shell)
{
	if (is_meta(input[*i]))
	{
		if (!handle_meta_character(input, i, tokens))
		{
			free_tokens(*tokens);
			return (false);
		}
	}
	else if (input[*i] == '\'' || input[*i] == '\"')
	{
		if (!handle_quoted_string(input, i, tokens, word_buf, shell))
		{
			free_tokens(*tokens);
			return (false);
		}
	}
	else
	{
		if (!handle_word(input, i, tokens, word_buf, shell))
		{
			free_tokens(*tokens);
			return (false);
		}
	}
	return (true);
}

bool	validate_tokens(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (!current->next || current->next->type == TOKEN_PIPE)
				return (false);
		}
		else if (current->type == TOKEN_REDIR_IN
			|| current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND
			|| current->type == TOKEN_HEREDOC)
		{
			if (!current->next || current->next->type != TOKEN_WORD)
				return (false);
		}
		current = current->next;
	}
	return (true);
}

t_token	*tokenize(char *input, t_minishell *shell)
{
	t_token	*tokens;
	char	word_buf[1024];
	int		i;

	if (!input)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (!process_token(input, &i, &tokens, word_buf, shell))
			return (NULL);
	}
	if (!tokens || !validate_tokens(tokens))
	{
		if (tokens)
			free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}
