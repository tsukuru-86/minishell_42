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

static int	open_heredoc_file(t_redirect *redir)
{
	if (access(redir->file, F_OK) != 0)
	{
		return (-1);
	}
	if (access(redir->file, R_OK) != 0)
	{
		return (-1);
	}
	return (open(redir->file, O_RDONLY));
}

static int	apply_in_redirect(t_redirect *redir)
{
	int	fd;

	if (redir->type == REDIR_HEREDOC)
		fd = open_heredoc_file(redir);
	else
		fd = open_redirect_file(redir);
	if (fd == -1)
	{
		return (0);
	}
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
