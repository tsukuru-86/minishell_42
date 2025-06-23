/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 02:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/24 00:04:51 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

/* Save the original file descriptor for later restoration */
int	save_original_fd(t_redirect *redirect)
{
	if (redirect->type == REDIR_OUT || redirect->type == REDIR_APPEND)
		return (dup(STDOUT_FILENO));
	if (redirect->type == REDIR_IN || redirect->type == REDIR_HEREDOC)
		return (dup(STDIN_FILENO));
	return (-1);
}

/* Check input file access */
static int	check_input_access(t_redirect *redirect)
{
	if (access(redirect->file, R_OK) != 0)
	{
		ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n", redirect->file,
			strerror(errno));
		set_env_node("?", "1");
		return (-1);
	}
	return (0);
}

/* Check file access permissions before opening */
int	check_file_access(t_redirect *redirect)
{
	if (redirect->type == REDIR_IN)
		return (check_input_access(redirect));
	if (access(redirect->file, F_OK) == 0)
	{
		if (access(redirect->file, W_OK) != 0)
		{
			ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n", redirect->file,
				strerror(errno));
			set_env_node("?", "1");
			return (-1);
		}
		return (0);
	}
	return (check_directory_access(redirect));
}

/* Open the file based on redirection type */
int	open_redirect_file(t_redirect *redirect)
{
	int	fd;

	fd = -1;
	if (redirect->type == REDIR_OUT)
		fd = open(redirect->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redirect->type == REDIR_APPEND)
		fd = open(redirect->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redirect->type == REDIR_HEREDOC)
	{
		debug_print_with_str("[DEBUG] Processing heredoc for file",
			redirect->file, DEBUG_ENABLED);
		fd = open(redirect->file, O_RDONLY);
		if (fd != -1)
			debug_print_with_int("[DEBUG] Heredoc file opened successfully, fd",
				fd, DEBUG_ENABLED);
	}
	else if (redirect->type == REDIR_IN)
		fd = open(redirect->file, O_RDONLY);
	if (fd == -1)
		ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n", redirect->file,
			strerror(errno));
	return (fd);
}
