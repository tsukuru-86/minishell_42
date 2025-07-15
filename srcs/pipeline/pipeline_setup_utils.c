/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_setup_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/06/09 16:48:44 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipeline.h"

/* Initialize pipe info for all commands in the pipeline */
void	init_pipeline(t_command *cmd)
{
	t_command	*current;

	if (!cmd)
		return ;
	current = cmd;
	while (current)
	{
		current->pipe.read_fd = -1;
		current->pipe.write_fd = -1;
		current->pipe.pid = -1;
		current = current->next;
	}
}

/* Create pipes between commands and set read/write file descriptors */
int	create_pipes(t_command *cmd)
{
	t_command	*current;
	int			pipefd[2];

	if (!cmd)
		return (0);
	current = cmd;
	while (current && current->next)
	{
		if (pipe(pipefd) == -1)
		{
			perror("minishell: pipe");
			cleanup_pipeline(cmd);
			return (0);
		}
		current->pipe.write_fd = pipefd[1];
		current->next->pipe.read_fd = pipefd[0];
		current = current->next;
	}
	return (1);
}

/* Close pipes in the parent process */
void	close_parent_pipes(t_command *cmd)
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
