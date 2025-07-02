/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 05:05:13 by muiida            #+#    #+#             */
/*   Updated: 2025/07/03 03:44:14 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

char	*get_temp_filename(void)
{
	static int	counter = 0;
	char		*pid_str;
	char		*temp;
	char		*result;

	pid_str = ft_itoa(getpid() + counter++);
	if (!pid_str)
		return (NULL);
	temp = ft_strjoin("/tmp/heredoc_", pid_str);
	free(pid_str);
	if (!temp)
		return (NULL);
	result = temp;
	return (result);
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
		free(filename);
		return (NULL);
	}
	close(fd);
	return (filename);
}

static void	parse_heredoc_delimiter(t_token *token, char **dst, bool *is_quoted)
{
	char	*content;
	int		len;

	debug_print_with_int("DEBUG: parsing delimiter token type", token->type);
	debug_print_with_str("DEBUG: parsing delimiter token content", token->content);
	content = token->content;
	len = ft_strlen(content);
	*is_quoted = false;
	
	if (len >= 2 && content[0] == '\'' && content[len - 1] == '\'')
	{
		debug_print("DEBUG: single quoted delimiter detected from content");
		*dst = ft_strdup(content);
		*is_quoted = true;
	}
	else if (len >= 2 && content[0] == '"' && content[len - 1] == '"')
	{
		debug_print("DEBUG: double quoted delimiter detected from content");
		*dst = ft_strdup(content);
		*is_quoted = true;
	}
	else
	{
		debug_print("DEBUG: unquoted delimiter detected");
		*dst = ft_strdup(content);
	}
	debug_print_with_str("DEBUG: final delimiter", *dst);
	debug_print_with_int("DEBUG: is_quoted", *is_quoted);
}

t_heredoc	*init_heredoc(t_token *delimiter_token)
{
	t_heredoc	*heredoc;

	heredoc = (t_heredoc *)malloc(sizeof(t_heredoc));
	if (!heredoc)
		return (NULL);
	parse_heredoc_delimiter(delimiter_token, &heredoc->delimiter,
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

