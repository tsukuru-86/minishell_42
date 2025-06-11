/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 04:02:00 by muiida           #+#    #+#             */
/*   Updated: 2025/06/01 04:02:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

/* Apply the input redirection */
int	process_redirections(t_redirect *redirect)
{
	t_redirect	*current;
	int			fd;

	current = redirect;
	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			fd = open_redirect_file(current);
			if (fd == -1)
				return (0);
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				close(fd);
				return (0);
			}
			close(fd);
		}
		else if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
		{
			fd = open_redirect_file(current);
			if (fd == -1)
				return (0);
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				close(fd);
				return (0);
			}
			close(fd);
		}
		current = current->next;
	}
	return (1);
}
