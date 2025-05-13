/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_child_env.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 05:13:14 by muiida            #+#    #+#             */
/*   Updated: 2025/05/12 05:16:23 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Set up child signals and pipeline environment */
void	setup_pipeline_child_env(void)
{
	setup_child_signals();
	putenv("MINISHELL_PIPELINE=1");
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
