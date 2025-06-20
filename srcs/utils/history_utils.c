/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:03:07 by muiida            #+#    #+#             */
/*   Updated: 2025/06/20 16:57:07 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	token = strtok_r(line, "\n", &saveptr);
	while (token)
	{
		add_history(token);
		token = strtok_r(NULL, "\n", &saveptr);
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
	if (fd < 0)
	{
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

/* OSによって異なる実装を提供 */
HIST_ENTRY	**get_history_entries(void);

static void	write_history_entries(int fd)
{
	HIST_ENTRY	**hist_list;
	int			i;

	hist_list = get_history_entries();
	i = 0;
	while (hist_list && hist_list[i])
	{
		write(fd, hist_list[i]->line, ft_strlen(hist_list[i]->line));
		write(fd, "\n", 1);
		i++;
	}
}

void	save_history_file(void)
{
	char	*path;
	int		fd;

	path = get_history_path();
	if (!path)
		return ;
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(path);
		return ;
	}
	write_history_entries(fd);
	close(fd);
	free(path);
}
