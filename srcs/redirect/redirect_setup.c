/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_setup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 04:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/06 08:56:48 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

static int	create_redirect_out_file(t_redirect *current)
{
	int	fd;

	fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd != -1)
	{
		close(fd);
		return (1);
	}
	else
	{
		ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n", current->file,
			strerror(errno));
	}
	return (0);
}

static int	create_redirect_append_file(t_redirect *current)
{
	int	fd;

	fd = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd != -1)
	{
		close(fd);
		return (1);
	}
	else
	{
		ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n", current->file,
			strerror(errno));
	}
	return (0);
}

static int	create_single_output_file(t_redirect *current)
{
	if (check_file_access(current) != -1)
	{
		if (current->type == REDIR_OUT)
			return (create_redirect_out_file(current));
		else if (current->type == REDIR_APPEND)
			return (create_redirect_append_file(current));
	}
	return (0);
}

// static int	create_output_files(t_redirect *redirect)
// {
// 	t_redirect	*current;

// 	current = redirect;
// 	while (current)
// 	{
// 		if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
// 		{
// 			if (!create_single_output_file(current))
// 				return (0);
// 		}
// 		current = current->next;
// 	}
// 	return (1);
// }

/* Set up all redirections in the chain */
int	setup_redirection(t_redirect *redirect)
{
	t_redirect	*current;

	current = redirect;
	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			if (!create_single_output_file(current))
				return (0);
		}
		else if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
		{
			if (!validate_input_redirect(current))
				return (0);
		}
		current = current->next;
	}
	save_original_fds(redirect);
	return (process_redirections(redirect));
}
