/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:18:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/16 05:43:48 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static char	*read_line_from_pipe(void)
{
	char	buf[4096];
	int		pos;
	char	c;
	int		r;

	pos = 0;
	while (pos < 4095)
	{
		r = read(STDIN_FILENO, &c, 1);
		if (r <= 0)
			break ;
		if (c == '\n')
			break ;
		buf[pos++] = c;
	}
	buf[pos] = '\0';
	if (pos == 0 && r <= 0)
		return (NULL);
	return (ft_strdup(buf));
}

int	read_heredoc_from_pipe(int fd, t_heredoc *heredoc)
{
	char	*line;
	int		result;

	while (1)
	{
		line = read_line_from_pipe();
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
