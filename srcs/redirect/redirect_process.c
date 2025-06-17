/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 04:02:00 by muiida           #+#    #+#             */
/*   Updated: 2025/06/01 04:02:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

static int	apply_out_redirect(t_redirect *redir)
{
	int	fd;

	fd = open_redirect_file(redir);
	if (fd == -1)
		return (0);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

static int	apply_in_redirect(t_redirect *redir)
{
	int	fd;

	if (redir->type == REDIR_HEREDOC)
	{
		debug_print_with_str("REDIR_HEREDOC file", redir->file, DEBUG_ENABLED);
		fd = open(redir->file, O_RDONLY);
		debug_print_with_int("REDIR_HEREDOC fd", fd, DEBUG_ENABLED);
	}
	else
	{
		fd = open_redirect_file(redir);
	}
	if (fd == -1)
		return (0);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

int	process_redirections(t_redirect *redirect)
{
	t_redirect	*current;

	current = redirect;
	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			if (!apply_out_redirect(current))
				return (0);
		}
		else if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
		{
			if (!apply_in_redirect(current))
				return (0);
		}
		current = current->next;
	}
	return (1);
}
