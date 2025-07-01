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
	debug_print("[DEBUG] restore_fds: Start.");
	if (last_out && last_out->original_fd != -1)
	{
		debug_print_with_int("[DEBUG] restore_fds: Restoring STDOUT from fd: ", last_out->original_fd);
		if (dup2(last_out->original_fd, STDOUT_FILENO) == -1)
			perror("minishell: dup2 STDOUT_FILENO restore");
		close(last_out->original_fd);
		debug_print_with_int("[DEBUG] restore_fds: Closed original_fd for STDOUT: ", last_out->original_fd);
		last_out->original_fd = -1;
	}
	if (last_in && last_in->original_fd != -1)
	{
		debug_print_with_int("[DEBUG] restore_fds: Restoring STDIN from fd: ", last_in->original_fd);
		if (dup2(last_in->original_fd, STDIN_FILENO) == -1)
			perror("minishell: dup2 STDIN_FILENO restore");
		close(last_in->original_fd);
		debug_print_with_int("[DEBUG] restore_fds: Closed original_fd for STDIN: ", last_in->original_fd);
		last_in->original_fd = -1;
	}
	debug_print("[DEBUG] restore_fds: Exit.");
}

/* Clean up heredoc files */
void	cleanup_heredocs(t_redirect *redirect)
{
	t_redirect	*current;

	debug_print("[DEBUG] cleanup_heredocs: Start.");
	current = redirect;
	while (current)
	{
		if (current->type == REDIR_HEREDOC)
		{
			debug_print_with_str("[DEBUG] cleanup_heredocs: Unlinking heredoc file: ", current->file);
			if (unlink(current->file) == -1)
				perror("minishell: unlink heredoc");
		}
		current = current->next;
	}
	debug_print("[DEBUG] cleanup_heredocs: Exit.");
}

void	restore_redirection(t_redirect *redirect)
{
	t_redirect	*last_out;
	t_redirect	*last_in;

	debug_print("[DEBUG] restore_redirection: Start.");
	if (!redirect)
	{
		debug_print("[DEBUG] restore_redirection: No redirects to restore, returning.");
		return ;
	}
	debug_print("[DEBUG] restore_redirection: Finding last redirections.");
	find_last_redirections(redirect, &last_out, &last_in);
	debug_print("[DEBUG] restore_redirection: Restoring file descriptors.");
	restore_fds(last_out, last_in);
	debug_print("[DEBUG] restore_redirection: Exit.");
}
