/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect_processing.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 02:34:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/15 02:34:54 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_redirect	*create_new_redirect(t_token **curr_token,
		t_redir_type redirect_type)
{
	t_redirect	*new_redirect;

	new_redirect = malloc(sizeof(t_redirect));
	if (!handle_memory_error(new_redirect, NULL))
		return (NULL);
	new_redirect->type = redirect_type;
	new_redirect->file = ft_strdup((*curr_token)->content);
	if (!handle_memory_error(new_redirect->file, NULL))
	{
		free(new_redirect);
		return (NULL);
	}
	new_redirect->next = NULL;
	return (new_redirect);
}

int	handle_redirect_token(t_token **curr_token, t_command *cmd)
{
	t_redirect		*new_redirect;
	t_redir_type	redirect_type;

	redirect_type = convert_token_type_to_redir_type((*curr_token)->type);
	*curr_token = (*curr_token)->next;
	if (*curr_token == NULL || (*curr_token)->type != TOKEN_WORD)
	{
		ft_printf_fd(2, "syntax error near unexpected token\n");
		return (RET_ERROR);
	}
	new_redirect = create_new_redirect(curr_token, redirect_type);
	if (!new_redirect)
		return (RET_ERROR);
	new_redirect->next = cmd->redirects;
	cmd->redirects = new_redirect;
	*curr_token = (*curr_token)->next;
	return (RET_SUCCESS);
}

int	handle_pipe_token(t_token **current, t_command ***cmds, int *cmd_idx)
{
	t_command	*new_command;

	if (!current || !cmds || !cmd_idx || !(*current))
		return (RET_ERROR);
	new_command = create_command();
	if (!handle_memory_error(new_command, NULL))
		return (RET_ERROR);
	(**cmds)->next = new_command;
	new_command->prev = **cmds;
	*cmds = &(**cmds)->next;
	*current = (*current)->next;
	(*cmd_idx)++;
	return (RET_SUCCESS);
}
