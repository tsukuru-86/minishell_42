/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_process_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 01:21:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/22 01:28:08 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

t_redirect	*find_last_output_redirect(t_redirect *redirect)
{
	t_redirect	*current;
	t_redirect	*last_out;

	current = redirect;
	last_out = NULL;
	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
			last_out = current;
		current = current->next;
	}
	return (last_out);
}

t_redirect	*find_last_input_redirect(t_redirect *redirect)
{
	t_redirect	*current;
	t_redirect	*last_in;

	current = redirect;
	last_in = NULL;
	while (current)
	{
		if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
			last_in = current;
		current = current->next;
	}
	return (last_in);
}

int	process_non_effective_redirects(t_redirect *redirect,
	t_redirect *last_out, t_redirect *last_in)
{
	t_redirect	*current;
	int			fd;

	current = redirect;
	while (current)
	{
		if (current != last_out && current != last_in)
		{
			if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
			{
				fd = open_redirect_file(current);
				if (fd == -1)
					return (0);
				close(fd);
			}
		}
		current = current->next;
	}
	return (1);
}
