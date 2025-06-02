/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 04:53:10 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/25 00:19:07 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

/* トークンの数をカウント */
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

/* 現在の文字に基づいてトークンを処理 */
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

/* 入力文字列をトークンに分割 */
t_token	*tokenize(char *input, t_command *cmd_param)
{
	t_tokenizer_stat	vars;

	if (!init_tokenizer_stat(&vars, cmd_param))
		return (NULL);
	if (!input)
	{
		finalize_tokenizer(&vars);
		return (NULL);
	}
	while (input[vars.i_input])
	{
		skip_whitespace(input, &vars.i_input);
		if (!input[vars.i_input])
			break ;
		if (!process_current_token(&vars, input))
			return (cleanup_and_return_null(&vars, input));
	}
	return (vars.tokens);
}
