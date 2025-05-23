/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:53:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/23 21:29:42 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	process_heredoc_line(char *line, int fd, t_heredoc *heredoc)
{
	char	*trimmed;
	int		is_delimiter;

	trimmed = ft_strtrim(line, " \t\n");
	if (!trimmed)
	{
		free(line);
		return (0);
	}
	is_delimiter = (ft_strncmp(trimmed, heredoc->delimiter,
				ft_strlen(heredoc->delimiter)) == 0
			&& ft_strlen(trimmed) == ft_strlen(heredoc->delimiter));
	free(trimmed);
	if (is_delimiter)
	{
		free(line);
		return (1);
	}
	if (!write_heredoc_content(fd, line))
	{
		free(line);
		return (0);
	}
	free(line);
	return (2);
}

static int	read_heredoc_input(int fd, t_heredoc *heredoc)
{
	char	*line;
	int		result;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		result = process_heredoc_line(line, fd, heredoc);
		if (result == 0)
			return (0);
		if (result == 1)
			break ;
	}
	return (1);
}

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

int	handle_heredoc(t_command *cmd, char *delimiter)
{
	t_heredoc	*heredoc;
	int			fd;

	printf("handle_heredoc delimiter: %s\n", delimiter);
	heredoc = init_heredoc(delimiter);
	if (!heredoc)
		return (0);
	fd = open(heredoc->temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		cleanup_heredoc(heredoc);
		return (0);
	}
	if (!read_heredoc_input(fd, heredoc))
	{
		close(fd);
		cleanup_heredoc(heredoc);
		return (0);
	}
	close(fd);
	return (finalize_heredoc(cmd, heredoc));
}
