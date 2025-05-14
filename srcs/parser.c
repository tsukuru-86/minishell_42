/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:13:41 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/15 01:22:20 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command	*initialize_command_list(void)
{
	t_command	*cmd;

	cmd = create_command();
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	cmd->pipe.read_fd = STDIN_FILENO;
	cmd->pipe.write_fd = STDOUT_FILENO;
	return (cmd);
}

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*cmd;
	t_command	*head;

	if (!tokens)
		return (NULL);
	cmd = initialize_command_list();
	if (!cmd)
		return (NULL);
	head = cmd;
	if (!process_tokens(&cmd, tokens, &head))
		return (NULL);
	return (head);
}
