/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_syntax_check_advanced.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 20:04:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/18 11:56:29 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/error_messages.h"
#include "minishell.h"
#include "tokenizer.h"

/*
** @brief Check if token is a redirect type
*/
static int	is_redirect_token_advanced(t_token_type type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

/*
** @brief Check for trailing redirect operator
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
** @brief Check for consecutive redirect operators
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
** @brief Perform advanced syntax check
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
