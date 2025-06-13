/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_validate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 01:12:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/13 20:04:05 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

int	validate_input_redirect(t_redirect *current)
{
	if (current->type == REDIR_IN)
	{
		if (access(current->file, F_OK) != 0)
		{
			ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n", current->file,
				strerror(errno));
			return (0);
		}
		if (access(current->file, R_OK) != 0)
		{
			ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n", current->file,
				strerror(errno));
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
