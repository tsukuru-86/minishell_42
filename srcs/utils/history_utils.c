/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:03:07 by muiida            #+#    #+#             */
/*   Updated: 2025/07/14 03:44:09 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

char	*get_history_path(void)
{
	char	*home;
	char	*path;
	size_t	len;

	home = getenv("HOME");
	if (!home)
		return (NULL);
	len = ft_strlen(home) + ft_strlen("/.minishell_history") + 1;
	path = (char *)malloc(len);
	if (!path)
		return (NULL);
	ft_strlcpy(path, home, len);
	ft_strlcat(path, "/.minishell_history", len);
	return (path);
}

static void	process_history_buffer(char *buf)
{
	char	*line;
	char	*saveptr;
	char	*token;

	line = buf;
	saveptr = NULL;
	token = ft_strtok_r(line, "\n", &saveptr);
	while (token)
	{
		add_history(token);
		token = ft_strtok_r(NULL, "\n", &saveptr);
	}
}

void	load_history_file(void)
{
	char	*path;
	int		fd;
	char	buf[4096];
	ssize_t	rlen;

	path = get_history_path();
	if (!path)
		return ;
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		perror(path);
		free(path);
		return ;
	}
	rlen = read(fd, buf, sizeof(buf) - 1);
	if (rlen > 0)
	{
		buf[rlen] = '\0';
		process_history_buffer(buf);
	}
	close(fd);
	free(path);
}
