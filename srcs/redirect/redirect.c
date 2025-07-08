/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 17:08:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/07/09 02:10:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

t_redirect	*create_redirect(int type, char *file)
{
	t_redirect	*redirect;
	char		*expanded_file;

	redirect = (t_redirect *)malloc(sizeof(t_redirect));
	if (!redirect)
		return (NULL);
	redirect->type = type;
	expanded_file = expand_redirect_filename(file);
	if (!expanded_file)
	{
		free(redirect);
		return (NULL);
	}
	redirect->file = expanded_file;
	redirect->original_fd = -1;
	redirect->next = NULL;
	return (redirect);
}

void	free_redirect(t_redirect *redirect)
{
	t_redirect	*next;

	if (!redirect)
		return ;
	next = redirect->next;
	if (redirect->file)
	{
		if (redirect->type == REDIR_HEREDOC)
			unlink(redirect->file);
		free(redirect->file);
	}
	free(redirect);
	if (next)
		free_redirect(next);
}
