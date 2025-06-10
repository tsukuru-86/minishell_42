/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:03:07 by muiida            #+#    #+#             */
/*   Updated: 2025/06/10 14:06:23 by muiida           ###   ########.fr       */
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
	len = strlen(home) + strlen("/.minishell_history") + 1;
	path = (char *)malloc(len);
	if (!path)
		return (NULL);
	strcpy(path, home);
	strcat(path, "/.minishell_history");
	return (path);
}

void	load_history_file(void)
{
	char	*path;
	int		fd;
	char	buf[4096];
	ssize_t	rlen;
	char	*line;
	char	*saveptr;
	char	*token;

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
		line = buf;
		saveptr = NULL;
		token = strtok_r(line, "\n", &saveptr);
		while (token)
		{
			add_history(token);
			token = strtok_r(NULL, "\n", &saveptr);
		}
	}
	close(fd);
	free(path);
}

void	save_history_file(void)
{
	char		*path;
	int			fd;
	HIST_ENTRY	**hist_list;
	int			i;

	path = get_history_path();
	if (!path)
		return ;
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(path);
		return ;
	}
	hist_list = history_list();
	i = 0;
	while (hist_list && hist_list[i])
	{
		write(fd, hist_list[i]->line, strlen(hist_list[i]->line));
		write(fd, "\n", 1);
		i++;
	}
	close(fd);
	free(path);
}
