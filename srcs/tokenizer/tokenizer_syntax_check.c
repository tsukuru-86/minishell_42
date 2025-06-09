/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_syntax_check.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 04:54:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/10 04:54:46 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../error/error_messages.h"
#include "minishell.h"
#include "tokenizer.h"

/*
** @brief パイプの連続をチェック
*/
static int	check_consecutive_pipes(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			next = current->next;
			while (next && next->type == TOKEN_SPACE)
				next = next->next;
			if (!next || next->type == TOKEN_PIPE)
			{
				ft_printf_fd(2, ERR_UNEXP_TOKEN, "|");
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}

/*
** @brief 先頭のパイプをチェック
*/
static int	check_leading_pipe(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current && current->type == TOKEN_SPACE)
		current = current->next;
	if (current && current->type == TOKEN_PIPE)
	{
		ft_printf_fd(2, ERR_UNEXP_TOKEN, "|");
		return (0);
	}
	return (1);
}

/*
** @brief リダイレクトタイプかチェック
*/
static int	is_redirect_token(t_token_type type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

/*
** @brief リダイレクト演算子の後をチェック
*/
static int	check_redirect_target(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		if (is_redirect_token(current->type))
		{
			next = current->next;
			while (next && next->type == TOKEN_SPACE)
				next = next->next;
			if (!next)
			{
				ft_printf_fd(2, ERR_UNEXP_TOKEN, "newline");
				return (0);
			}
			if (next->type == TOKEN_PIPE || is_redirect_token(next->type))
			{
				ft_printf_fd(2, ERR_UNEXP_TOKEN, next->content);
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}

/*
** @brief 基本的な構文チェックを実行
*/
int	check_basic_syntax(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (!check_leading_pipe(tokens))
		return (0);
	if (!check_consecutive_pipes(tokens))
		return (0);
	if (!check_redirect_target(tokens))
		return (0);
	return (1);
}