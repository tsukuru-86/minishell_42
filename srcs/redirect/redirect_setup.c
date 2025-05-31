/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_setup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 04:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/01 04:56:51 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

/* Open all output files to ensure they are created */
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
			fd = open_redirect_file(current);
			if (fd == -1)
				return (0);
			close(fd);
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
