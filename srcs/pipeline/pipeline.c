/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:36:29 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/14 20:05:12 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipeline.h"

/* パイプラインの完了を待機 */
int	wait_pipeline(t_command *cmd)
{
	t_command	*current;
	int			status;
	int			last_status;

	if (!cmd)
		return (0);
	current = cmd;
	last_status = 0;
	while (current)
	{
		if (current->pipe.pid != -1)
		{
			if (waitpid(current->pipe.pid, &status, 0) == -1)
			{
				perror("minishell: waitpid");
				last_status = 1;
			}
			else if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		current = current->next;
	}
	return (last_status);
}
