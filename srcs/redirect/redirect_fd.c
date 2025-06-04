/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 00:56:00 by muiida           #+#    #+#             */
/*   Updated: 2025/06/04 00:56:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"
#include <unistd.h>

int	save_original_fd(t_redirect *redirect)
{
	if (redirect->type == REDIR_OUT || redirect->type == REDIR_APPEND)
		return (dup(STDOUT_FILENO));
	if (redirect->type == REDIR_IN || redirect->type == REDIR_HEREDOC)
		return (dup(STDIN_FILENO));
	return (-1);
}

int	apply_redirection(t_redirect *redirect, int fd)
{
	int	target_fd;

	if (fd == -1)
		return (0);
	if (redirect->type == REDIR_OUT || redirect->type == REDIR_APPEND)
		target_fd = STDOUT_FILENO;
	else if (redirect->type == REDIR_IN || redirect->type == REDIR_HEREDOC)
		target_fd = STDIN_FILENO;
	else
		target_fd = -1;
	if (target_fd == -1)
	{
		close(fd);
		return (0);
	}
	if (dup2(fd, target_fd) == -1)
	{
		close(fd);
		ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n", redirect->file,
			strerror(errno));
		return (0);
	}
	close(fd);
	return (1);
}

// /* Apply the redirection using the file descriptor */
// void	apply_redirection(t_redirect *redirect, int fd)
// {
// 	if (redirect->type == REDIR_OUT || redirect->type == REDIR_APPEND)
// 		dup2(fd, STDOUT_FILENO);
// 	else if (redirect->type == REDIR_IN || redirect->type == REDIR_HEREDOC)
// 		dup2(fd, STDIN_FILENO);
// 	if (fd == -1)
// 		perror("minishell: apply_redirection");
// 	close(fd);
// }
