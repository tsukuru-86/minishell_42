/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_input_processing.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 00:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/17 00:00:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static int	count_tokens(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static int	process_current_token(t_tokenizer_stat *stat, const char *input)
{
	if (count_tokens(stat->tokens) >= MAX_TOKENS)
	{
		ft_putstr_fd((char *)"minishell: too many tokens\n", STDERR_FILENO);
		return (0);
	}
	if (is_meta(input[stat->i_input]))
	{
		if (!handle_meta_token_creation(stat, input))
			return (0);
	}
	else
	{
		if (!handle_word_token_creation(stat, input))
			return (0);
		finalize_tokenizer(stat);
	}
	return (1);
}

int	process_input_char(t_tokenizer_stat *vars, const char *input)
{
	if (input[vars->i_input] == '\n')
	{
		add_token_to_list(&vars->tokens, create_token(ft_strdup("\n"),
				TOKEN_NEWLINE));
		vars->i_input++;
		return (1);
	}
	else if (is_delimiter(input[vars->i_input]))
	{
		if (!handle_space_token_creation(vars, input))
			return (0);
		return (1);
	}
	else if (!process_current_token(vars, input))
		return (0);
	return (1);
}
