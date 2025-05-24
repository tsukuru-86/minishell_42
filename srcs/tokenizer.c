/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 04:53:10 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/24 21:47:10 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

/* 現在の文字に基づいてトークンを処理 */
static int	process_current_token(t_tokenizer_stat *vars, const char *input)
{
	if (is_quote(input[vars->i]))
	{
		if (!handle_quoted_token_creation(vars, input))
			return (0);
	}
	else if (is_meta(input[vars->i]))
	{
		if (!handle_meta_token_creation(vars, input))
			return (0);
	}
	else
	{
		if (!handle_word_token_creation(vars, input))
			return (0);
		finalize_tokenizer(vars);
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
	while (input[vars.i])
	{
		skip_whitespace(input, &vars.i);
		if (!input[vars.i])
			break ;
		if (!process_current_token(&vars, input))
			return (cleanup_and_return_null(&vars, input));
	}
	return (vars.tokens);
}
