/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 08:59:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/20 08:59:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils/input_utils.h"

void	handle_heredoc_error(t_command **head_cmd)
{
	if (*head_cmd)
		free_command(*head_cmd);
	*head_cmd = NULL;
}

void	skip_to_delimiter(t_token **current_token, const char *delimiter)
{
	*current_token = (*current_token)->next;
	while (*current_token)
	{
		if ((*current_token)->type == TOKEN_WORD
			&& ft_strcmp((*current_token)->content, delimiter) == 0)
		{
			*current_token = (*current_token)->next;
			break ;
		}
		*current_token = (*current_token)->next;
	}
}

int	handle_interactive_heredoc(t_command *cmd, t_token *delimiter_token,
		t_command **head_cmd, t_token **current_token)
{
	debug_print("[DEBUG] Interactive mode", DEBUG_ENABLED);
	if (!handle_heredoc(cmd, delimiter_token->content))
	{
		handle_heredoc_error(head_cmd);
		return (0);
	}
	*current_token = delimiter_token->next;
	return (1);
}

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

static int	write_heredoc_from_stdin(t_command *cmd, char *delimiter)
{
	t_heredoc	*heredoc;
	int			fd;

	heredoc = init_heredoc(delimiter);
	if (!heredoc)
		return (0);
	fd = open(heredoc->temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		cleanup_heredoc(heredoc);
		return (0);
	}
	process_heredoc_lines(fd, delimiter);
	close(fd);
	if (cmd->redirects)
		free_redirect(cmd->redirects);
	cmd->redirects = create_redirect(REDIR_HEREDOC, heredoc->temp_file);
	cleanup_heredoc(heredoc);
	return (1);
}

int	handle_noninteractive_heredoc(t_command *cmd, t_token *delimiter_token,
		t_command **head_cmd, t_token **current_token)
{
	debug_print("[DEBUG] Non-interactive mode with stdin reading",
		DEBUG_ENABLED);
	if (!write_heredoc_from_stdin(cmd, delimiter_token->content))
	{
		handle_heredoc_error(head_cmd);
		return (0);
	}
	*current_token = delimiter_token->next;
	return (1);
}
