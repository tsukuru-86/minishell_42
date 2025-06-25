/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 06:24:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/25 21:36:28 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

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
		debug_print("[DEBUG] non-tty mode heredoc");
		return (process_pipe_heredoc_lines(fd, heredoc));
	}
}
