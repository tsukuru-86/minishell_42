/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_syntax_check_advanced.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 20:04:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/14 20:04:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/error_messages.h"
#include "minishell.h"
#include "tokenizer.h"

/*
** @brief リダイレクトタイプかチェック
*/
static int	is_redirect_token_advanced(t_token_type type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

/*
** @brief 末尾のリダイレクト演算子をチェック
*/
static int	check_trailing_redirect(t_token *tokens)
{
	t_token	*current;
	t_token	*last_non_space;

	if (!tokens)
		return (1);
	last_non_space = NULL;
	current = tokens;
	while (current)
	{
		if (current->type != TOKEN_SPACE)
			last_non_space = current;
		current = current->next;
	}
	if (last_non_space && is_redirect_token_advanced(last_non_space->type))
	{
		ft_printf_fd(2, ERR_UNEXP_TOKEN, "newline");
		return (0);
	}
	return (1);
}

/*
** @brief 連続するリダイレクト演算子をチェック
*/
static int	check_consecutive_redirects(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		if (is_redirect_token_advanced(current->type))
		{
			next = current->next;
			while (next && next->type == TOKEN_SPACE)
				next = next->next;
			if (next && is_redirect_token_advanced(next->type))
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
** @brief 高度な構文チェックを実行
*/
int	check_advanced_syntax(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (!check_trailing_redirect(tokens))
		return (0);
	if (!check_consecutive_redirects(tokens))
		return (0);
	return (1);
}