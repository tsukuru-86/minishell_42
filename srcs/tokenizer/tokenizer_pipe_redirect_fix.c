/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_pipe_redirect_fix.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 11:50:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/09 02:43:25 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"
#include "error/error_messages.h"

/*
** @brief パイプ後のリダイレクションかチェック
*/
int	is_pipe_redirect(t_token *prev, t_token *current)
{
	if (prev && prev->type == TOKEN_PIPE && (current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND
			|| current->type == TOKEN_REDIR_IN
			|| current->type == TOKEN_HEREDOC))
		return (1);
	return (0);
}

/*
** @brief リダイレクト演算子のターゲット検証
*/
int	validate_redirect_target(t_token *current, t_token *prev)
{
	t_token	*next;

	next = current->next;
	while (next && next->type == TOKEN_SPACE)
		next = next->next;
	if (!next && !is_pipe_redirect(prev, current))
	{
		ft_printf_fd(2, ERR_UNEXP_TOKEN, "newline");
		return (0);
	}
	if (next && (next->type == TOKEN_PIPE || (next->type == TOKEN_REDIR_IN
				|| next->type == TOKEN_REDIR_OUT
				|| next->type == TOKEN_REDIR_APPEND
				|| next->type == TOKEN_HEREDOC)))
	{
		ft_printf_fd(2, ERR_UNEXP_TOKEN, next->content);
		return (0);
	}
	return (1);
}

/*
** @brief パイプ・リダイレクト組み合わせを考慮した構文チェック
*/
int	check_pipe_redirect_syntax(t_token *tokens)
{
	t_token	*current;
	t_token	*prev;

	current = tokens;
	prev = NULL;
	while (current)
	{
		if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND
			|| current->type == TOKEN_HEREDOC)
		{
			if (!validate_redirect_target(current, prev))
				return (0);
		}
		prev = current;
		current = current->next;
	}
	return (1);
}
