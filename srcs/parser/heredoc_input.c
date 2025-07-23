/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 06:24:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/23 16:39:12 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "debug/debug.h"

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

int	read_heredoc_input(int fd, t_heredoc *heredoc)
{
	if (isatty(STDIN_FILENO))
		return (read_heredoc_tty(fd, heredoc));
	else
	{
		dbg_printf("non-tty mode heredoc");
		return (process_pipe_heredoc_lines(fd, heredoc));
	}
}
