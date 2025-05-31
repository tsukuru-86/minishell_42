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

/* Find the last output and input redirections in the chain */
static void	find_last_redirections_process(t_redirect *redirect,
			t_redirect **last_out, t_redirect **last_in)
{
	t_redirect	*current;

	*last_out = NULL;
	*last_in = NULL;
	current = redirect;
	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
			*last_out = current;
		else if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
			*last_in = current;
		current = current->next;
	}
}

/* Apply the output redirection */
static int	apply_output_redirection(t_redirect *last_out)
{
	int	fd;

	if (!last_out)
		return (1);
	fd = open_redirect_file(last_out);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(last_out->file, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (0);
	}
	apply_redirection(last_out, fd);
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
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(last_in->file, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (0);
	}
	apply_redirection(last_in, fd);
	return (1);
}

/* Process redirections in order, applying only the last effective one */
int	process_redirections(t_redirect *redirect)
{
	t_redirect	*last_out;
	t_redirect	*last_in;

	find_last_redirections_process(redirect, &last_out, &last_in);
	if (!apply_output_redirection(last_out))
		return (0);
	if (!apply_input_redirection(last_in))
		return (0);
	return (1);
}
