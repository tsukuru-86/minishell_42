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
static void	save_stdout_redirect(t_redirect *current, int *has_stdout_redirect)
{
	if ((current->type == REDIR_OUT || current->type == REDIR_APPEND)
		&& !*has_stdout_redirect)
	{
		current->original_fd = save_original_fd(current);
		*has_stdout_redirect = 1;
	}
}

/* Save stdin redirection */
static void	save_stdin_redirect(t_redirect *current, int *has_stdin_redirect)
{
	if ((current->type == REDIR_IN || current->type == REDIR_HEREDOC)
		&& !*has_stdin_redirect)
	{
		current->original_fd = save_original_fd(current);
		*has_stdin_redirect = 1;
	}
}

/* Save original file descriptors for later restoration */
void	save_original_fds(t_redirect *redirect)
{
	t_redirect	*current;
	int			has_stdout_redirect;
	int			has_stdin_redirect;

	has_stdout_redirect = 0;
	has_stdin_redirect = 0;
	current = redirect;
	while (current)
	{
		save_stdout_redirect(current, &has_stdout_redirect);
		save_stdin_redirect(current, &has_stdin_redirect);
		current = current->next;
	}
}
