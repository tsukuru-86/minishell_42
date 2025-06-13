/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:53:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/13 19:50:25 by muiida           ###   ########.fr       */
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
	if (!isatty(STDIN_FILENO))
		return (read_heredoc_from_pipe(fd, heredoc));
	else
		return (read_heredoc_input(fd, heredoc));
}

int	handle_heredoc(t_command *cmd, char *delimiter)
{
	t_heredoc	*heredoc;
	int			fd;

	heredoc = init_heredoc(delimiter);
	if (!heredoc)
	{
		return (0);
	}
	fd = open(heredoc->temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		cleanup_heredoc(heredoc);
		return (0);
	}
	if (!process_heredoc_content(fd, heredoc))
	{
		close(fd);
		cleanup_heredoc(heredoc);
		return (0);
	}
	close(fd);
	return (finalize_heredoc(cmd, heredoc));
}
