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

static t_redirect	*find_last_output_redirect(t_redirect *redirect)
{
	t_redirect	*current;
	t_redirect	*last_out;

	current = redirect;
	last_out = NULL;
	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
			last_out = current;
		current = current->next;
	}
	return (last_out);
}

static t_redirect	*find_last_input_redirect(t_redirect *redirect)
{
	t_redirect	*current;
	t_redirect	*last_in;

	current = redirect;
	last_in = NULL;
	while (current)
	{
		if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
			last_in = current;
		current = current->next;
	}
	return (last_in);
}

static int	process_non_effective_redirects(t_redirect *redirect,
	t_redirect *last_out, t_redirect *last_in)
{
	t_redirect	*current;
	int			fd;

	current = redirect;
	while (current)
	{
		if (current != last_out && current != last_in)
		{
			if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
			{
				fd = open_redirect_file(current);
				if (fd == -1)
					return (0);
				close(fd);
			}
		}
		current = current->next;
	}
	return (1);
}

int	process_redirections(t_redirect *redirect)
{
	t_redirect	*last_out;
	t_redirect	*last_in;

	if (!redirect)
		return (1);
	last_out = find_last_output_redirect(redirect);
	last_in = find_last_input_redirect(redirect);
	if (!process_non_effective_redirects(redirect, last_out, last_in))
		return (0);
	if (last_out && !apply_out_redirect(last_out))
		return (0);
	if (last_in && !apply_in_redirect(last_in))
		return (0);
	return (1);
}
