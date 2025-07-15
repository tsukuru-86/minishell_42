/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_cleanup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:51:42 by muiida            #+#    #+#             */
/*   Updated: 2025/06/02 03:51:58 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipeline.h"

/* Cleanup the pipeline */
void	cleanup_pipeline(t_command *cmd)
{
	t_command	*current;

	if (!cmd)
		return ;
	current = cmd;
	while (current)
	{
		if (current->pipe.read_fd != -1)
		{
			close(current->pipe.read_fd);
			current->pipe.read_fd = -1;
		}
		if (current->pipe.write_fd != -1)
		{
			close(current->pipe.write_fd);
			current->pipe.write_fd = -1;
		}
		current = current->next;
	}
}

/* Cleanup all pipeline commands */
void	cleanup_pipeline_list_all(t_command *cmd)
{
	t_command	*current;

	current = cmd;
	while (current)
	{
		cleanup_pipeline(current);
		current = current->next;
	}
}
