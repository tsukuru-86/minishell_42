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
#include "debug/debug.h"

static int	apply_out_redirect(t_redirect *redir)
{
	int	fd;

	fd = open_redirect_file(redir);
	if (fd == -1)
		return (0);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

static int	open_heredoc_file(t_redirect *redir)
{
	int	fd;

	dbg_printf("REDIR_HEREDOC file=%s", redir->file);
	if (access(redir->file, F_OK) == -1)
	{
		perror(redir->file);
		return (-1);
	}
	if (access(redir->file, R_OK) == -1)
	{
		perror(redir->file);
		return (-1);
	}
	{
		fd = open(redir->file, O_RDONLY);
		if (fd == -1)
		{
			dbg_printf("open(O_RDONLY) failed=%s", redir->file);
			perror(redir->file);
		}
		return (fd);
	}
}

static int	apply_in_redirect(t_redirect *redir)
{
	int	fd;

	if (redir->type == REDIR_HEREDOC)
		fd = open_heredoc_file(redir);
	else
		fd = open_redirect_file(redir);
	if (fd == -1)
		return (0);
	dbg_printf("apply_in_redirect: dup2 STDIN%d", fd);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		dbg_printf("apply_in_redirect: dup2 failed");
		close(fd);
		return (0);
	}
	close(fd);
	dbg_printf("apply_in_redirect: success");
	return (1);
}

static int	check_all_input_files(t_redirect *redirect)
{
	t_redirect	*cur;
	int			fd;

	cur = redirect;
	while (cur)
	{
		if (cur->type == REDIR_IN || cur->type == REDIR_HEREDOC)
		{
			if (!cur->file || cur->file[0] == '\0')
				return (ft_printf_fd(STDERR_FILENO, "minishell: null: %s\n",
						strerror(errno)));
			fd = open(cur->file, O_RDONLY);
			if (fd == -1)
				return (ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n",
						cur->file, strerror(errno)));
			close(fd);
		}
		cur = cur->next;
	}
	return (1);
}

int	process_redirections(t_redirect *redirect)
{
	t_redirect	*last_out;
	t_redirect	*last_in;

	if (!redirect)
		return (1);
	if (!check_all_input_files(redirect))
		return (0);
	last_out = find_last_output_redirect(redirect);
	last_in = find_last_input_redirect(redirect);
	if (!skip_non_effective_redirects(redirect, last_out, last_in))
		return (0);
	if (last_out && !apply_out_redirect(last_out))
		return (0);
	if (last_in && !apply_in_redirect(last_in))
		return (0);
	return (1);
}
