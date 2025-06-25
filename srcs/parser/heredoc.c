/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:53:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/25 21:48:05 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/*
 * heredoc 構造体に関するリソースをクリーンアップする。
 * heredoc->temp_fileはunlink されない。リダイレクトによって所有される。
 * 成功した場合は1を、失敗した場合は0を返す。
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
		debug_print("[DEBUG] Non-interactive heredoc");
		return (process_pipe_heredoc_lines(fd, heredoc));
	}
}

static int	process_heredoc_file(int fd, t_heredoc *heredoc)
{
	debug_print("[DEBUG] handle_heredoc: calling process_heredoc_content");
	if (!process_heredoc_content(fd, heredoc))
	{
		debug_print("[DEBUG] handle_heredoc: process_heredoc_content failed");
		close(fd);
		cleanup_heredoc(heredoc);
		return (0);
	}
	debug_print("[DEBUG] handle_heredoc: process_heredoc_content succeeded");
	return (1);
}

static int	create_heredoc_file(t_heredoc *heredoc)
{
	int	fd;

	fd = open(heredoc->temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		debug_print("[DEBUG] handle_heredoc: open failed");
		cleanup_heredoc(heredoc);
		return (-1);
	}
	if (!process_heredoc_file(fd, heredoc))
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

	debug_print_with_str("[DEBUG] handle_heredoc: delimiter", delimiter);
	heredoc = init_heredoc(delimiter);
	if (!heredoc)
		return (0);
	debug_print_with_str("[DEBUG] handle_heredoc: temp_file",
		heredoc->temp_file);
	if (create_heredoc_file(heredoc) == -1)
		return (0);
	return (finalize_heredoc(cmd, heredoc));
}
