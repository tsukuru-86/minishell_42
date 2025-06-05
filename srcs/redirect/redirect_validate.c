/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_validate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 01:12:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/06 01:11:56 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

static int	validate_input_redirect(t_redirect *current)
{
	if (current->type == REDIR_IN)
	{
		if (access(current->file, F_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(current->file, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return (0);
		}
		if (access(current->file, R_OK) != 0)
		{
			ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n",
				current->file, strerror(errno));
			return (0);
		}
	}
	return (1);
}

int	validate_redirections(t_redirect *redirect)
{
	t_redirect	*current;
	int			has_valid_input;

	current = redirect;
	has_valid_input = 1;
	while (current)
	{
		if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
		{
			if (!validate_input_redirect(current))
				has_valid_input = 0;
		}
		current = current->next;
	}
	return (has_valid_input);
}
