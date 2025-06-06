/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 21:38:31 by muiida            #+#    #+#             */
/*   Updated: 2025/06/07 03:13:42 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*split_line(char *line)
{
	size_t	count;
	char	*leftover;

	count = 0;
	while (line[count] != '\n' && line[count] != '\0')
		count++;
	if (line[count] == '\0')
		return (0);
	leftover = ft_substr(line, count + 1, ft_strlen(line) - count);
	if (leftover == NULL)
		return (NULL);
	else if (*leftover == '\0')
	{
		free(leftover);
		return (NULL);
	}
	line[count + 1] = '\0';
	return (leftover);
}

static char	*extend_line(char *s1, char *s2)
{
	char	*tmp;

	if (s1 == NULL)
		s1 = ft_strdup("");
	tmp = s1;
	s1 = ft_strjoin(s1, s2);
	free(tmp);
	return (s1);
}

static char	*read_file_until_break(int fd, char **leftover)
{
	int		read_cnt;
	char	*buffer;

	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (buffer == NULL)
		return (NULL);
	read_cnt = 1;
	while (0 < read_cnt)
	{
		read_cnt = read(fd, buffer, BUFFER_SIZE);
		if (read_cnt == -1)
		{
			free(*leftover);
			*leftover = NULL;
			break ;
		}
		else if (read_cnt == 0)
			break ;
		buffer[read_cnt] = '\0';
		*leftover = extend_line(*leftover, buffer);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	free(buffer);
	return (*leftover);
}

char	*get_next_line(int fd)
{
	static char	*leftover;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = read_file_until_break(fd, &leftover);
	if (line == NULL)
		return (NULL);
	leftover = split_line(line);
	return (line);
}

void	clear_gnl_buffer(int fd)
{
	char	*tmp;

	tmp = get_next_line(fd);
	while (tmp != NULL)
	{
		free(tmp);
		tmp = get_next_line(fd);
	}
}
