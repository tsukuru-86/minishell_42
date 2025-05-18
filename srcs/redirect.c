/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 17:08:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/19 01:22:34 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <stdio.h>

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
	return (redirect);
}

/* Set up redirection by saving, opening file, and applying it */
int	setup_redirection(t_redirect *redirect)
{
	int	fd;

	if (!redirect)
		return (0);
	redirect->original_fd = save_original_fd(redirect);
	fd = open_redirect_file(redirect);
	if (fd == -1)
	{
		ft_putstr_fd((char *)"minishell: ", 2);
		ft_putstr_fd(redirect->file, 2);
		ft_putstr_fd((char *)": ", 2);
		perror("");
		return (0);
	}
	apply_redirection(redirect, fd);
	return (1);
}

void	restore_redirection(t_redirect *redirect)
{
	if (!redirect || redirect->original_fd == -1)
		return ;
	if (redirect->type == REDIR_OUT || redirect->type == REDIR_APPEND)
		dup2(redirect->original_fd, STDOUT_FILENO);
	else if (redirect->type == REDIR_IN)
		dup2(redirect->original_fd, STDIN_FILENO);
	close(redirect->original_fd);
	redirect->original_fd = -1;
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
