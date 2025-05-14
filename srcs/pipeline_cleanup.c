/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_cleanup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:50:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/14 22:15:59 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

/* パイプラインの完了を待機 */
int	wait_pipeline(t_command *cmd)
{
	t_command	*current;
	int			status;
	int			last_status;

	current = cmd;
	last_status = 0;
	while (current)
	{
		if (current->pipe.pid != -1)
		{
			if (waitpid(current->pipe.pid, &status, 0) == -1)
			{
				perror("waitpid");
				return (-1);
			}
			else if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
		}
		current = current->next;
	}
	return (last_status);
}

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
