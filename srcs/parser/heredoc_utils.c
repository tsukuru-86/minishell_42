/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 05:05:13 by muiida            #+#    #+#             */
/*   Updated: 2025/07/24 22:10:45 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "libft/libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

static void	reverse_str(char *str, int len)
{
	int		i;
	char	tmp;

	i = 0;
	while (i < len / 2)
	{
		tmp = str[i];
		str[i] = str[len - 1 - i];
		str[len - 1 - i] = tmp;
		i++;
	}
}

char	*get_temp_filename(void)
{
	static int	counter = 0;
	char		numstr[16];
	int			len;
	int			n;
	char		*temp;

	len = 0;
	n = counter++;
	ft_memset(numstr, 0, sizeof(numstr));
	if (n == 0)
		numstr[len++] = '0';
	else
	{
		while (n > 0 && len < 15)
		{
			numstr[len++] = '0' + (n % 10);
			n /= 10;
		}
	}
	reverse_str(numstr, len);
	temp = ft_strjoin("/tmp/heredoc_", numstr);
	if (!temp)
		return (NULL);
	return (temp);
}

int	write_heredoc_content(int fd, char *content)
{
	if (write(fd, content, ft_strlen(content)) == -1)
		return (0);
	if (write(fd, "\n", 1) == -1)
		return (0);
	return (1);
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
		perror(filename);
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
