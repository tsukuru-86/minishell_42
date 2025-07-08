/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 06:50:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/03 21:12:38 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	process_unquoted_line(char *line, int fd)
{
	char	*expanded;

	debug_print_with_str("DEBUG: processing line", line);
	expanded = expand_env_vars(line, 1);
	if (!expanded)
	{
		free(line);
		return (0);
	}
	debug_print_with_str("DEBUG: expanded", expanded);
	if (!write_heredoc_content(fd, expanded))
	{
		free(expanded);
		free(line);
		return (0);
	}
	free(expanded);
	return (1);
}

static int	process_quoted_line(char *line, int fd)
{
	debug_print_with_str("DEBUG: processing quoted line (no expansion)", line);
	if (!write_heredoc_content(fd, line))
	{
		free(line);
		return (0);
	}
	return (1);
}

static int	check_delimiter_match(char *line, t_heredoc *heredoc)
{
	return (ft_strcmp(line, heredoc->delimiter) == 0);
}

int	process_heredoc_line(char *line, int fd, t_heredoc *heredoc)
{
	int	is_delimiter;
	int	result;

	debug_print_with_str("DEBUG: heredoc line", line);
	debug_print_with_str("DEBUG: delimiter", heredoc->delimiter);
	debug_print_with_int("DEBUG: is_quoted", heredoc->delimiter_is_quoted);
	is_delimiter = check_delimiter_match(line, heredoc);
	debug_print_with_int("DEBUG: is_delimiter", is_delimiter);
	if (is_delimiter)
	{
		free(line);
		return (1);
	}
	if (heredoc->delimiter_is_quoted)
		result = process_quoted_line(line, fd);
	else
		result = process_unquoted_line(line, fd);
	free(line);
	if (!result)
		return (0);
	return (2);
}
