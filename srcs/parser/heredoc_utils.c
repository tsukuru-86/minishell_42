/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 05:05:13 by muiida            #+#    #+#             */
/*   Updated: 2025/06/11 06:39:54 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

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

static void	parse_heredoc_delimiter(char *src, char **dst, bool *is_quoted)
{
	size_t	len;

	len = ft_strlen(src);
	if (len >= 2 && ((src[0] == '\'' && src[len - 1] == '\'') || (src[0] == '\"'
				&& src[len - 1] == '\"')))
	{
		*dst = ft_substr(src, 1, len - 2);
		*is_quoted = true;
	}
	else
	{
		*dst = ft_strdup(src);
		*is_quoted = false;
	}
}

t_heredoc	*init_heredoc(char *delimiter)
{
	t_heredoc	*heredoc;

	heredoc = (t_heredoc *)malloc(sizeof(t_heredoc));
	if (!heredoc)
		return (NULL);
	parse_heredoc_delimiter(delimiter, &heredoc->delimiter,
		&heredoc->delimiter_is_quoted);
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
