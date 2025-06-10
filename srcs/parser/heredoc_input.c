/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 06:24:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/11 06:48:33 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	read_char_until_newline(char *buf)
{
	int		pos;
	char	c;
	int		r;

	pos = 0;
	while (pos < 4095)
	{
		r = read(0, &c, 1);
		if (r <= 0)
			break ;
		if (c == '\n')
			break ;
		buf[pos++] = c;
	}
	return (pos);
}

static char	*read_line_from_stdin(void)
{
	char	buf[4096];
	int		pos;

	pos = read_char_until_newline(buf);
	if (pos == 0)
		return (NULL);
	buf[pos] = '\0';
	return (ft_strdup(buf));
}

int	read_heredoc_tty(int fd, t_heredoc *heredoc)
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

int	read_heredoc_non_tty(int fd, t_heredoc *heredoc)
{
	char	*line;
	int		result;

	while (1)
	{
		line = read_line_from_stdin();
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

int	read_heredoc_input(int fd, t_heredoc *heredoc)
{
	if (isatty(0))
		return (read_heredoc_tty(fd, heredoc));
	else
		return (read_heredoc_non_tty(fd, heredoc));
}
