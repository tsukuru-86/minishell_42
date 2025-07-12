/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_save_fds.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 04:03:00 by muiida           #+#    #+#             */
/*   Updated: 2025/06/01 04:03:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

/* Save stdout redirection */
static void	save_stdout_redirect(t_redirect *current, t_redirect **last_stdout)
{
	if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		*last_stdout = current;
}

/* Save stdin redirection */
static void	save_stdin_redirect(t_redirect *current, t_redirect **last_stdin)
{
	if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
		*last_stdin = current;
}

/* Save original file descriptors for later restoration */
void	save_original_fds(t_redirect *redirect)
{
	t_redirect	*current;
	t_redirect	*last_stdout;
	t_redirect	*last_stdin;

	last_stdout = NULL;
	last_stdin = NULL;
	current = redirect;
	while (current)
	{
		save_stdout_redirect(current, &last_stdout);
		save_stdin_redirect(current, &last_stdin);
		current = current->next;
	}
	if (last_stdout)
		last_stdout->original_fd = save_original_fd(last_stdout);
	if (last_stdin)
		last_stdin->original_fd = save_original_fd(last_stdin);
}
