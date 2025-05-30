/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 17:08:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/31 01:12:22 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

t_redirect	*create_redirect(int type, char *file)
{
	t_redirect	*redirect;

	redirect = (t_redirect *)malloc(sizeof(t_redirect));
	if (!redirect)
		return (NULL);
	redirect->type = type;
	redirect->file = ft_strdup(file);
	if (!redirect->file)
	{
		free(redirect);
		return (NULL);
	}
	redirect->original_fd = -1;
	redirect->next = NULL;
	return (redirect);
}

/* Save original file descriptors for later restoration */
static void	save_original_fds(t_redirect *redirect)
{
	t_redirect	*current;

	current = redirect;
	while (current)
	{
		if (current->original_fd == -1)
			current->original_fd = save_original_fd(current);
		current = current->next;
	}
}

/* Check if a redirection for the same fd exists earlier in the chain */
static int	has_earlier_redirect_for_fd(t_redirect *current, t_redirect *redirect_list)
{
	t_redirect	*check;

	check = redirect_list;
	while (check && check != current)
	{
		if ((current->type == REDIR_OUT || current->type == REDIR_APPEND) &&
			(check->type == REDIR_OUT || check->type == REDIR_APPEND))
			return (1);
		if ((current->type == REDIR_IN || current->type == REDIR_HEREDOC) &&
			(check->type == REDIR_IN || check->type == REDIR_HEREDOC))
			return (1);
		check = check->next;
	}
	return (0);
}

/* Set up all redirections in the chain */
int	setup_redirection(t_redirect *redirect)
{
	t_redirect	*current;
	int			fd;

	if (!redirect)
		return (1);
	save_original_fds(redirect);
	current = redirect;
	while (current)
	{
		if (!has_earlier_redirect_for_fd(current, redirect))
		{
			fd = open_redirect_file(current);
			if (fd == -1)
			{
				ft_putstr_fd((char *)"minishell: ", 2);
				ft_putstr_fd(current->file, 2);
				ft_putstr_fd((char *)": ", 2);
				perror("");
				return (0);
			}
			apply_redirection(current, fd);
		}
		current = current->next;
	}
	return (1);
}

void	restore_redirection(t_redirect *redirect)
{
	t_redirect	*current;

	if (!redirect)
		return ;
	current = redirect;
	while (current)
	{
		if (current->original_fd != -1)
		{
			if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
				dup2(current->original_fd, STDOUT_FILENO);
			else if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
				dup2(current->original_fd, STDIN_FILENO);
			close(current->original_fd);
			current->original_fd = -1;
		}
		if (current->type == REDIR_HEREDOC)
			unlink(current->file);
		current = current->next;
	}
}

void	free_redirect(t_redirect *redirect)
{
	if (!redirect)
		return ;
	if (redirect->file)
		free(redirect->file);
	if (redirect->original_fd != -1)
		close(redirect->original_fd);
	free(redirect);
}
