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

static void	find_last_valid_redirections(t_redirect *redirect,
		t_redirect **last_out, t_redirect **last_in)
{
	t_redirect	*current;

	*last_out = NULL;
	*last_in = NULL;
	current = redirect;
	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			if (check_file_access(current) != -1)
				*last_out = current;
		}
		else if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
		{
			if (current->type == REDIR_HEREDOC)
				*last_in = current;
			else if (access(current->file, R_OK) == 0)
				*last_in = current;
		}
		current = current->next;
	}
}

static int	try_open_output_file(t_redirect *redirect, int *fd)
{
	*fd = open_redirect_file(redirect);
	if (*fd == -1)
	{
		if (check_file_access(redirect) == -1)
			return (0);
		return (0);
	}
	return (1);
}

static int	apply_output_redirection(t_redirect *last_out)
{
	int	fd;

	if (!last_out)
		return (1);
	if (!try_open_output_file(last_out, &fd))
		return (0);
	apply_output_redirection_fd(last_out, fd);
	return (1);
}

/* Apply the input redirection */
static int	apply_input_redirection(t_redirect *last_in)
{
	int	fd;

	if (!last_in)
		return (1);
	fd = open_redirect_file(last_in);
	if (fd == -1)
		return (0);
	apply_input_redirection_fd(fd);
	return (1);
}

int	process_redirections(t_redirect *redirect)
{
	t_redirect	*last_out;
	t_redirect	*last_in;

	find_last_valid_redirections(redirect, &last_out, &last_in);
	if (!apply_output_redirection(last_out))
		return (0);
	if (!apply_input_redirection(last_in))
		return (0);
	return (1);
}
