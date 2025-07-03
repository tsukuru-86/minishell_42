/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_utils3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ai <ai@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 21:47:00 by ai                #+#    #+#             */
/*   Updated: 2025/06/23 21:47:00 by ai               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	is_delimiter_line(char *line, char *delimiter)
{
	return (ft_strcmp(line, delimiter) == 0);
}

static int	process_heredoc_lines(int fd, t_heredoc *heredoc)
{
	char	*line;
	char	*expanded;

	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (is_delimiter_line(line, heredoc->delimiter))
		{
			free(line);
			break ;
		}
		if (heredoc->delimiter_is_quoted && heredoc->delimiter[0] == '\'')
		{
			write(fd, line, ft_strlen(line));
			write(fd, "\n", 1);
		}
		else
		{
			expanded = expand_env_vars(line, 1);
			if (expanded)
			{
				write(fd, expanded, ft_strlen(expanded));
				free(expanded);
			}
			else
				write(fd, line, ft_strlen(line));
			write(fd, "\n", 1);
		}
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	return (1);
}

int	write_heredoc_from_stdin(t_command *cmd, t_token *delimiter_token)
{
	t_heredoc	*heredoc;
	int			fd;

	heredoc = init_heredoc(delimiter_token);
	if (!heredoc)
		return (0);
	fd = open(heredoc->temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		cleanup_heredoc(heredoc);
		return (0);
	}
	process_heredoc_lines(fd, heredoc);
	close(fd);
	if (cmd->redirects)
		free_redirect(cmd->redirects);
	cmd->redirects = create_redirect(REDIR_HEREDOC, heredoc->temp_file);
	cleanup_heredoc(heredoc);
	return (1);
}
