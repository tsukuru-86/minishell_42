/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 04:53:10 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/17 08:34:55 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

t_token	*safe_create_token(char *content, t_token_type type)
{
	t_token	*token;

	if (!content)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->content = ft_strdup(content);
	if (!token->content)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

/* Cleanup on syntax error */
static t_token	*handle_syntax_error(t_tokenizer_stat *vars)
{
	set_exit_status(NULL, 2);
	free_tokens(vars->tokens);
	if (vars->needs_cmd_free && vars->cmd)
	{
		free(vars->cmd);
		vars->cmd = NULL;
	}
	return (NULL);
}

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
		if (!process_input_char(&vars, input))
			return (cleanup_and_return_null(&vars, input));
	}
	if (!check_basic_syntax(vars.tokens))
		return (handle_syntax_error(&vars));
	finalize_tokenizer(&vars);
	return (vars.tokens);
}
