/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_validate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 01:12:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/09 02:08:41 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect.h"

static int	is_invalid_input_device(const char *filename)
{
	if (ft_strcmp(filename, "/dev/stdout") == 0)
		return (1);
	if (ft_strcmp(filename, "/dev/stderr") == 0)
		return (1);
	return (0);
}

int	validate_input_redirect(t_redirect *current)
{
	if (!check_filename_limit(current->file))
		return (0);
	if (current->type == REDIR_IN)
	{
		if (is_invalid_input_device(current->file))
		{
			set_env_node("?", "1");
			return (0);
		}
		if (access(current->file, F_OK) != 0)
		{
			ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n", current->file,
				strerror(errno));
			set_env_node("?", "1");
			return (0);
		}
		if (access(current->file, R_OK) != 0)
		{
			ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n", current->file,
				strerror(errno));
			set_env_node("?", "1");
			return (0);
		}
	}
	return (1);
}

int	validate_redirections(t_redirect *redirect)
{
	t_redirect	*current;

	current = redirect;
	while (current)
	{
		if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
		{
			if (!validate_input_redirect(current))
				return (0);
		}
		current = current->next;
	}
	return (1);
}
