/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 05:13:14 by muiida            #+#    #+#             */
/*   Updated: 2025/06/09 16:48:57 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
#include "pipeline.h"

/* Set up child signals and pipeline environment */
/* Close all pipe file descriptors in the command list */
void	pipeline_close_pipes(t_command *cmd)
{
	t_command	*tmp;

	if (!cmd)
		return ;
	tmp = cmd;
	while (tmp)
	{
		if (tmp->pipe.read_fd != -1)
		{
			close(tmp->pipe.read_fd);
			tmp->pipe.read_fd = -1;
		}
		if (tmp->pipe.write_fd != -1)
		{
			close(tmp->pipe.write_fd);
			tmp->pipe.write_fd = -1;
		}
		tmp = tmp->next;
	}
}
