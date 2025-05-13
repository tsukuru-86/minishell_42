/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 02:00:00 by muiida           #+#    #+#             */
/*   Updated: 2025/05/11 02:00:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

/* Save the original file descriptor for later restoration */
int	save_original_fd(t_redirect *redirect)
{
	if (redirect->type == REDIR_OUT || redirect->type == REDIR_APPEND)
		return (dup(STDOUT_FILENO));
	if (redirect->type == REDIR_IN)
		return (dup(STDIN_FILENO));
	return (-1);
}

/* Open the file based on redirection type */
int	open_redirect_file(t_redirect *redirect)
{
	if (redirect->type == REDIR_OUT)
		return (open(redirect->file, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (redirect->type == REDIR_APPEND)
		return (open(redirect->file, O_WRONLY | O_CREAT | O_APPEND, 0644));
	if (redirect->type == REDIR_IN)
		return (open(redirect->file, O_RDONLY));
	return (-1);
}

/* Apply the redirection using the file descriptor */
void	apply_redirection(t_redirect *redirect, int fd)
{
	if (redirect->type == REDIR_OUT || redirect->type == REDIR_APPEND)
		dup2(fd, STDOUT_FILENO);
	else if (redirect->type == REDIR_IN)
		dup2(fd, STDIN_FILENO);
	close(fd);
}
