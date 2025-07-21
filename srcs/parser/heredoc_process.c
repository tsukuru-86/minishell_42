/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 06:50:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/16 04:29:14 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils/debug.h"

// static int	process_quoted_line(char *line, int fd)
// {
// 	if (!write_heredoc_content(fd, line))
// 	{
// 		free(line);
// 		return (0);
// 	}
// 	return (1);
// }

static int	process_unquoted_line(char *line, int fd)
{
	char	*expanded;

	expanded = expand_env_vars(line, 1);
	if (!expanded)
	{
		free(line);
		return (0);
	}
	if (!write_heredoc_content(fd, expanded))
	{
		free(expanded);
		free(line);
		return (0);
	}
	free(expanded);
	return (1);
}

int	process_heredoc_line(char *line, int fd, t_heredoc *heredoc)
{
	int	is_delimiter;
	int	result;

	dbg_printf("heredoc compare: delimiter=%s", heredoc->delimiter);
	dbg_printf("heredoc compare: line=%s", line);
	is_delimiter = (ft_strcmp(line, heredoc->delimiter) == 0);
	if (is_delimiter)
	{
		free(line);
		return (1);
	}
	result = process_unquoted_line(line, fd);
	free(line);
	if (!result)
		return (0);
	return (2);
}
