/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 02:00:00 by muiida           #+#    #+#             */
/*   Updated: 2025/06/01 03:48:00 by muiida           ###   ########.fr       */
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

/* Check file access permissions before opening */
int	check_file_access(t_redirect *redirect)
{
	if (redirect->type == REDIR_OUT || redirect->type == REDIR_APPEND)
	{
		if (access(redirect->file, F_OK) == 0)
			return (access(redirect->file, W_OK));
		return (access(".", W_OK));
	}
	if (redirect->type == REDIR_IN)
		return (access(redirect->file, R_OK));
	return (0);
}

/* Open the file based on redirection type */
int	open_redirect_file(t_redirect *redirect)
{
	if (redirect->type == REDIR_OUT)
		return (open(redirect->file, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (redirect->type == REDIR_APPEND)
		return (open(redirect->file, O_WRONLY | O_CREAT | O_APPEND, 0644));
	if (redirect->type == REDIR_IN || redirect->type == REDIR_HEREDOC)
		return (open(redirect->file, O_RDONLY));
	return (-1);
}

/* Apply the redirection using the file descriptor */
void	apply_redirection(t_redirect *redirect, int fd)
{
	if (redirect->type == REDIR_OUT || redirect->type == REDIR_APPEND)
		dup2(fd, STDOUT_FILENO);
	else if (redirect->type == REDIR_IN || redirect->type == REDIR_HEREDOC)
		dup2(fd, STDIN_FILENO);
	close(fd);
}

/* Validate all redirections before processing */
int	validate_redirections(t_redirect *redirect)
{
	t_redirect	*current;

	current = redirect;
	while (current)
	{
		if (current->type == REDIR_IN)
		{
			if (access(current->file, F_OK) != 0)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(current->file, 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				return (0);
			}
		}
		if (check_file_access(current) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(current->file, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			return (0);
		}
		current = current->next;
	}
	return (1);
}
