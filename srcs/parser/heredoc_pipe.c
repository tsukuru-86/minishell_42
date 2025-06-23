/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:18:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/24 00:10:26 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	fill_pipe_buffer(t_pipe_buffer *pb)
{
	int	bytes_read;

	pb->pos = 0;
	pb->line_start = 0;
	bytes_read = read(STDIN_FILENO, pb->buffer, PIPE_BUFFER_SIZE - 1);
	if (bytes_read <= 0)
	{
		pb->size = 0;
		return (0);
	}
	if (!check_pipe_buffer_limit(bytes_read))
	{
		pb->size = 0;
		return (0);
	}
	pb->buffer[bytes_read] = '\0';
	pb->size = bytes_read;
	return (1);
}

static char	*extract_line_from_buffer(t_pipe_buffer *pb)
{
	char	*line;
	int		line_len;
	int		i;

	i = pb->pos;
	while (i < pb->size && pb->buffer[i] != '\n')
		i++;
	line_len = i - pb->pos;
	line = malloc(line_len + 1);
	if (!line)
		return (NULL);
	ft_memcpy(line, &pb->buffer[pb->pos], line_len);
	line[line_len] = '\0';
	if (i < pb->size && pb->buffer[i] == '\n')
		pb->pos = i + 1;
	else
		pb->pos = i;
	return (line);
}

static char	*read_line_from_pipe(void)
{
	static t_pipe_buffer	pb = {0};
	char					*line;

	if (pb.pos >= pb.size)
	{
		if (!fill_pipe_buffer(&pb))
			return (NULL);
	}
	line = extract_line_from_buffer(&pb);
	debug_print_with_str("[DEBUG] read_line_from_pipe: final line", line,
		DEBUG_ENABLED);
	return (line);
}

static int	process_pipe_heredoc_lines(int fd, t_heredoc *heredoc)
{
	char	*line;
	int		result;

	while (1)
	{
		line = read_line_from_pipe();
		if (!line)
			break ;
		debug_print_with_str("[DEBUG] Processing heredoc line", line,
			DEBUG_ENABLED);
		result = process_heredoc_line(line, fd, heredoc);
		if (result == 0)
			return (0);
		if (result == 1)
			break ;
	}
	return (1);
}

int	read_heredoc_from_pipe(int fd, t_heredoc *heredoc)
{
	debug_print("[DEBUG] read_heredoc_from_pipe: starting", DEBUG_ENABLED);
	return (process_pipe_heredoc_lines(fd, heredoc));
}
