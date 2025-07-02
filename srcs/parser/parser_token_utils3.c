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
	size_t	del_len;

	del_len = ft_strlen(delimiter);
	if (ft_strncmp(line, delimiter, del_len) == 0)
	{
		if (line[del_len] == '\n' || line[del_len] == '\0')
			return (1);
	}
	return (0);
}

static int	process_heredoc_lines(int fd, char *delimiter)
{
	char	*line;
	char	*expanded;

	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		if (is_delimiter_line(line, delimiter))
		{
			free(line);
			break ;
		}
		expanded = expand_env_vars(line, 1);
		if (expanded)
		{
			write(fd, expanded, ft_strlen(expanded));
			free(expanded);
		}
		else
			write(fd, line, ft_strlen(line));
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
	process_heredoc_lines(fd, heredoc->delimiter);
	close(fd);
	if (cmd->redirects)
		free_redirect(cmd->redirects);
	cmd->redirects = create_redirect(REDIR_HEREDOC, heredoc->temp_file);
	cleanup_heredoc(heredoc);
	return (1);
}
