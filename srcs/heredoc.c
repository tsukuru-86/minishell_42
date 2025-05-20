/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:53:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/19 19:53:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_temp_filename(void)
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

char	*create_heredoc_file(void)
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
		unlink(heredoc->temp_file);
		free(heredoc->temp_file);
	}
	free(heredoc);
}

static int	write_heredoc_content(int fd, char *content)
{
	if (write(fd, content, ft_strlen(content)) == -1)
		return (0);
	if (write(fd, "\n", 1) == -1)
		return (0);
	return (1);
}

int	handle_heredoc(t_command *cmd, char *delimiter)
{
	t_heredoc	*heredoc;
	char		*line;
	int			fd;

	heredoc = (t_heredoc *)malloc(sizeof(t_heredoc));
	if (!heredoc)
		return (0);
	heredoc->delimiter = ft_strdup(delimiter);
	heredoc->temp_file = create_heredoc_file();
	if (!heredoc->delimiter || !heredoc->temp_file)
	{
		cleanup_heredoc(heredoc);
		return (0);
	}
	fd = open(heredoc->temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		cleanup_heredoc(heredoc);
		return (0);
	}
	while (1)
	{
		line = readline("> ");
		char *trimmed;
		int is_delimiter;

		if (!line)
			break;
		trimmed = ft_strtrim(line, " \t\n");
		if (!trimmed)
		{
			free(line);
			close(fd);
			cleanup_heredoc(heredoc);
			return (0);
		}
		is_delimiter = (ft_strncmp(trimmed, heredoc->delimiter, ft_strlen(heredoc->delimiter)) == 0
			&& ft_strlen(trimmed) == ft_strlen(heredoc->delimiter));
		free(trimmed);
		if (is_delimiter)
		{
			free(line);
			break ;
		}
		if (!write_heredoc_content(fd, line))
		{
			free(line);
			close(fd);
			cleanup_heredoc(heredoc);
			return (0);
		}
		free(line);
	}
	close(fd);
	if (cmd->redirects)
		free_redirect(cmd->redirects);
	cmd->redirects = create_redirect(REDIR_HEREDOC, heredoc->temp_file);
	if (!cmd->redirects)
	{
		unlink(heredoc->temp_file);
		cleanup_heredoc(heredoc);
		return (0);
	}
	free(heredoc->delimiter);
	free(heredoc);
	return (1);
}