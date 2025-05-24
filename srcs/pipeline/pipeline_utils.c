/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 05:13:14 by muiida            #+#    #+#             */
/*   Updated: 2025/05/25 02:42:24 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipeline.h"

/* Set up child signals and pipeline environment */
/* Close all pipe file descriptors in the command list */
void	pipeline_close_pipes(t_command *cmd)
{
	t_command	*tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->pipe.read_fd != -1)
			close(tmp->pipe.read_fd);
		if (tmp->pipe.write_fd != -1)
			close(tmp->pipe.write_fd);
		tmp = tmp->next;
	}
}
