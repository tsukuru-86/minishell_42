/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 21:32:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/16 21:32:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/error_messages.h"
#include "minishell.h"
#include "parser.h"
#include "utils/debug.h"

/*
** @brief Handle redirect tokens.
** @param cmd Current command structure
** @param current_token Pointer to the current token
** @param head_cmd Pointer to the head of the command list
** @return 1 on success, 0 on failure
*/
static int	handle_normal_redirect(t_command *cmd, t_token **current_token,
		t_command **head_cmd)
{
	if (!add_redirect(cmd, *current_token, (*current_token)->next))
	{
		if (*head_cmd)
		{
			free_command(*head_cmd);
		}
		ft_putstr_fd((char *)ERR_REDIRECTION_ERROR, 2);
		return (0);
	}
	*current_token = (*current_token)->next;
	if (*current_token)
		*current_token = (*current_token)->next;
	return (1);
}

static int	handle_empty_cmd_redirect(t_command *cmd, t_token **current_token,
		t_command **head_cmd)
{
	dbg_printf("Empty command with redirect");
	if ((*current_token)->type == TOKEN_HEREDOC)
		return (handle_heredoc_redirect(cmd, current_token, head_cmd));
	if (!add_redirect(cmd, *current_token, (*current_token)->next))
	{
		if (*head_cmd)
			free_command(*head_cmd);
		ft_putstr_fd((char *)ERR_REDIRECTION_ERROR, 2);
		return (0);
	}
	*current_token = (*current_token)->next;
	if (*current_token)
		*current_token = (*current_token)->next;
	return (1);
}

int	handle_redirect_token(t_command *cmd, t_token **current_token,
		t_command **head_cmd)
{
	if (!cmd || !current_token || !*current_token || !head_cmd)
		return (0);
	if (!cmd->args || !cmd->args[0])
	{
		if ((*current_token)->type == TOKEN_HEREDOC)
			return (handle_heredoc_with_lookahead(cmd, current_token,
					head_cmd));
		return (handle_empty_cmd_redirect(cmd, current_token, head_cmd));
	}
	if (!((*current_token)->next))
	{
		if (*head_cmd)
			free_command(*head_cmd);
		ft_printf_fd(2, ERR_UNEXP_TOKEN, "newline");
		*head_cmd = NULL;
		return (0);
	}
	if ((*current_token)->type == TOKEN_HEREDOC)
		return (handle_heredoc_redirect(cmd, current_token, head_cmd));
	return (handle_normal_redirect(cmd, current_token, head_cmd));
}
