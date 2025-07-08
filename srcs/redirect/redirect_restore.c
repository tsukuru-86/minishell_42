/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_restore.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 04:02:00 by muiida           #+#    #+#             */
/*   Updated: 2025/06/01 04:02:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
#include "redirect.h"

/* Find the last output and input redirections */
static void	find_last_redirections(t_redirect *redirect, t_redirect **last_out,
		t_redirect **last_in)
{
	t_redirect	*current;

	*last_out = NULL;
	*last_in = NULL;
	current = redirect;
	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
			*last_out = current;
		else if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
			*last_in = current;
		current = current->next;
	}
}

/* Restore file descriptors */
static void	restore_fds(t_redirect *last_out, t_redirect *last_in)
{
	if (last_out && last_out->original_fd != -1)
	{
		if (dup2(last_out->original_fd, STDOUT_FILENO) == -1)
			perror("minishell: dup2 STDOUT_FILENO restore");
		close(last_out->original_fd);
		last_out->original_fd = -1;
	}
	if (last_in && last_in->original_fd != -1)
	{
		if (dup2(last_in->original_fd, STDIN_FILENO) == -1)
			perror("minishell: dup2 STDIN_FILENO restore");
		close(last_in->original_fd);
		last_in->original_fd = -1;
	}
}

/* Clean up heredoc files */
void	cleanup_heredocs(t_redirect *redirect)
{
	t_redirect	*current;

	current = redirect;
	while (current)
	{
		if (current->type == REDIR_HEREDOC)
		{
			if (unlink(current->file) == -1)
				perror("minishell: unlink heredoc");
		}
		current = current->next;
	}
}

void	restore_redirection(t_redirect *redirect)
{
	t_redirect	*last_out;
	t_redirect	*last_in;

	if (!redirect)
	{
		return ;
	}
	find_last_redirections(redirect, &last_out, &last_in);
	restore_fds(last_out, last_in);
}
