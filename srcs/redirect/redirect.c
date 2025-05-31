/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 17:08:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/01 02:29:59 by muiida           ###   ########.fr       */
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
	int			has_stdout_redirect;
	int			has_stdin_redirect;

	has_stdout_redirect = 0;
	has_stdin_redirect = 0;
	current = redirect;
	while (current)
	{
		if ((current->type == REDIR_OUT || current->type == REDIR_APPEND)
			&& !has_stdout_redirect)
		{
			current->original_fd = save_original_fd(current);
			has_stdout_redirect = 1;
		}
		else if ((current->type == REDIR_IN || current->type == REDIR_HEREDOC)
			&& !has_stdin_redirect)
		{
			current->original_fd = save_original_fd(current);
			has_stdin_redirect = 1;
		}
		current = current->next;
	}
}

/* Check if a redirection for the same fd exists later in the chain */
static int	has_later_redirect_for_fd(t_redirect *current)
{
	t_redirect	*check;

	check = current->next;
	while (check)
	{
		if ((current->type == REDIR_OUT || current->type == REDIR_APPEND)
			&& (check->type == REDIR_OUT || check->type == REDIR_APPEND))
			return (1);
		if ((current->type == REDIR_IN || current->type == REDIR_HEREDOC)
			&& (check->type == REDIR_IN || check->type == REDIR_HEREDOC))
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
		fd = open_redirect_file(current);
		if (fd == -1)
		{
			ft_putstr_fd((char *)"minishell: redirection error: ", 2);
			ft_putstr_fd(current->file, 2);
			perror("");
			return (0);
		}
		if (!has_later_redirect_for_fd(current))
			apply_redirection(current, fd);
		else
			close(fd);
		current = current->next;
	}
	return (1);
}

void	restore_redirection(t_redirect *redirect)
{
	t_redirect	*current;
	t_redirect	*last_out;
	t_redirect	*last_in;

	if (!redirect)
		return ;
	last_out = NULL;
	last_in = NULL;
	current = redirect;
	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
			last_out = current;
		else if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
			last_in = current;
		current = current->next;
	}
	if (last_out && last_out->original_fd != -1)
	{
		dup2(last_out->original_fd, STDOUT_FILENO);
		close(last_out->original_fd);
		last_out->original_fd = -1;
	}
	if (last_in && last_in->original_fd != -1)
	{
		dup2(last_in->original_fd, STDIN_FILENO);
		close(last_in->original_fd);
		last_in->original_fd = -1;
	}
	current = redirect;
	while (current)
	{
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
