/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_setup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 04:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/05 00:26:28 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

/* Create all output files but don't write to them yet
   Create/truncate file for REDIR_OUT, just check for REDIR_APPEND */
static int	create_output_files(t_redirect *redirect)
{
	t_redirect	*current;
	int			fd;

	current = redirect;
	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			if (check_file_access(current) == -1)
				return (0);
			if (current->type == REDIR_OUT)
			{
				fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (fd == -1)
				{
					ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n",
						current->file, strerror(errno));
					return (0);
				}
				close(fd);
			}
		}
		current = current->next;
	}
	return (1);
}

/* Set up all redirections in the chain */
int	setup_redirection(t_redirect *redirect)
{
	if (!redirect)
		return (1);
	if (!validate_redirections(redirect))
		return (0);
	if (!create_output_files(redirect))
		return (0);
	save_original_fds(redirect);
	return (process_redirections(redirect));
}
