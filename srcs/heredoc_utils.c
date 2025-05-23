/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 05:05:13 by muiida            #+#    #+#             */
/*   Updated: 2025/05/23 05:11:53 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_temp_filename(void)
{
	static int	counter = 0;
	char		*num_str;
	char		*filename;

	num_str = ft_itoa(counter++);
	if (!num_str)
		return (NULL);
	filename = ft_strjoin("/tmp/heredoc_", num_str);
	free(num_str);
	return (filename);
}

static char	*create_heredoc_file(void)
{
	char	*filename;
	int		fd;

	filename = get_temp_filename();
	if (!filename)
		return (NULL);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(filename);
		return (NULL);
	}
	close(fd);
	return (filename);
}

t_heredoc	*init_heredoc(char *delimiter)
{
	t_heredoc	*heredoc;

	heredoc = (t_heredoc *)malloc(sizeof(t_heredoc));
	if (!heredoc)
		return (NULL);
	heredoc->delimiter = ft_strdup(delimiter);
	heredoc->temp_file = create_heredoc_file();
	heredoc->content = NULL;
	heredoc->is_closed = true;
	if (!heredoc->delimiter || !heredoc->temp_file)
	{
		cleanup_heredoc(heredoc);
		return (NULL);
	}
	return (heredoc);
}

int	write_heredoc_content(int fd, char *content)
{
	if (write(fd, content, ft_strlen(content)) == -1)
		return (0);
	if (write(fd, "\n", 1) == -1)
		return (0);
	return (1);
}

void	cleanup_heredoc(t_heredoc *heredoc)
{
	if (!heredoc)
		return ;
	if (heredoc->delimiter)
		free(heredoc->delimiter);
	if (heredoc->content)
		free(heredoc->content);
	if (heredoc->temp_file)
	{
		if (!heredoc->is_closed)
		{
			unlink(heredoc->temp_file);
			heredoc->is_closed = true;
		}
		free(heredoc->temp_file);
	}
	free(heredoc);
}
