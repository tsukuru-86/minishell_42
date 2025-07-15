/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands_exec2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:10:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/20 09:10:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "external/external.h"
#include "minishell.h"
#include "utils/utils.h"

int	handle_empty_redirect(t_command *cmd)
{
	pid_t	pid;

	if (cmd->redirects != NULL)
	{
		pid = fork();
		if (pid == -1)
			return (1);
		if (pid == 0)
		{
			if (setup_redirection(cmd->redirects))
				exit(0);
			exit(1);
		}
		return (wait_parent(pid));
	}
	return (127);
}
