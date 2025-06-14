/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 04:53:10 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/14 13:47:46 by muiida           ###   ########.fr       */
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

/* 構文エラー時のクリーンアップ処理 */
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
		if (is_delimiter(input[vars.i_input]))
		{
			if (!handle_space_token_creation(&vars, input))
				return (cleanup_and_return_null(&vars, input));
		}
		else if (!process_current_token(&vars, input))
			return (cleanup_and_return_null(&vars, input));
	}
	if (!check_basic_syntax(vars.tokens))
		return (handle_syntax_error(&vars));
	finalize_tokenizer(&vars);
	return (vars.tokens);
}
