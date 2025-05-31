/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:36:29 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/01 02:00:37 by muiida           ###   ########.fr       */
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

	current = cmd;
	last_status = 0;
	while (current)
	{
		if (current->pipe.pid != -1)
		{
			if (waitpid(current->pipe.pid, &status, 0) == -1)
				perror("waitpid");
			else if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
		}
		current = current->next;
	}
	return (last_status);
}
