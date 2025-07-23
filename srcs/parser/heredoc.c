/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:53:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/23 16:39:12 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "debug/debug.h"

/*
 * Clean up resources related to the heredoc structure.
 * heredoc->temp_file is not unlinked; it is owned by the redirect.
 * Returns 1 on success, 0 on failure.
 */
static int	finalize_heredoc(t_command *cmd, t_heredoc *heredoc)
{
	if (cmd->redirects)
		free_redirect(cmd->redirects);
	cmd->redirects = create_redirect(REDIR_HEREDOC, heredoc->temp_file);
	if (!cmd->redirects)
	{
		cleanup_heredoc(heredoc);
		return (0);
	}
	if (heredoc->delimiter)
		free(heredoc->delimiter);
	heredoc->delimiter = NULL;
	if (heredoc->temp_file)
		free(heredoc->temp_file);
	heredoc->temp_file = NULL;
	free(heredoc);
	return (1);
}

static int	process_heredoc_content(int fd, t_heredoc *heredoc)
{
	if (isatty(STDIN_FILENO))
		return (read_heredoc_input(fd, heredoc));
	else
	{
		dbg_printf("Non-interactive heredoc");
		return (process_pipe_heredoc_lines(fd, heredoc));
	}
}

static int	process_heredoc_file(int fd, t_heredoc *heredoc)
{
	dbg_printf("handle_heredoc: calling process_heredoc_content");
	if (!process_heredoc_content(fd, heredoc))
	{
		dbg_printf("handle_heredoc: process_heredoc_content failed");
		close(fd);
		cleanup_heredoc(heredoc);
		return (0);
	}
	dbg_printf("handle_heredoc: process_heredoc_content succeeded");
	return (1);
}

static int	create_heredoc_file(t_heredoc *heredoc)
{
	int	fd;

	fd = open(heredoc->temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(heredoc->temp_file);
		dbg_printf("handle_heredoc: open failed");
		cleanup_heredoc(heredoc);
		return (-1);
	}
	else if (!process_heredoc_file(fd, heredoc))
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_heredoc(t_command *cmd, char *delimiter)
{
	t_heredoc	*heredoc;

	dbg_printf("handle_heredoc: delimiter=%s", delimiter);
	heredoc = init_heredoc(delimiter);
	if (!heredoc)
		return (0);
	dbg_printf("handle_heredoc: temp_file=%s", heredoc->temp_file);
	if (create_heredoc_file(heredoc) == -1)
		return (0);
	return (finalize_heredoc(cmd, heredoc));
}
