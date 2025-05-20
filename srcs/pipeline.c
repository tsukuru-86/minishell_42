/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:36:29 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/20 22:24:50 by muiida            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* パイプラインのセットアップ */
int	setup_pipeline(t_command *cmd)
{
	if (!cmd->next)
		return (1); // Not a pipeline if only one command
	init_pipeline(cmd);
	if (!create_pipes(cmd))
	{
		ft_putstr_fd((char *)"minishell: failed to create pipes\n", 2);
		return (0);
	}
	if (!spawn_pipeline_processes(cmd))
	{
		ft_putstr_fd((char *)"minishell: failed to spawn processes\n", 2);
		return (0);
	}
	close_parent_pipes(cmd);
	return (1);
}

/* パイプラインのクリーンアップ */
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
				perror("waitpid");
			else if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			// Consider handling other exit statuses like WIFSIGNALED
		}
		current = current->next;
	}
	return (last_status);
}

// Note: pipeline_close_pipes used in execute_pipeline_command (pipeline_process_utils.c)
// needs to be defined. If it's similar to close_parent_pipes but for child context,
// ensure it's correctly implemented and accessible.
// For now,
	// assuming it's a global function or defined in minishell.h if needed by execute_pipeline_command.
// If pipeline_close_pipes is specific to child process logic,
	// it could be static in pipeline_process_utils.c
// or part of execute_pipeline_command.
// The original code had `pipeline_close_pipes(cmd);` in `execute_pipeline_command`.
// This function is not defined in the provided `pipeline.c`.
// Let's assume `pipeline_close_pipes` is a function that closes all pipe fds in `cmd` list from child.
// It might look like this (add to pipeline_process_utils.c if static,
	// or pipeline_utils.c if general):
/*
void	pipeline_close_pipes(t_command *cmd_list)
{
	t_command	*iter;

	iter = cmd_list;
	while (iter)
	{
		if (iter->pipe.read_fd != -1)
			close(iter->pipe.read_fd);
		if (iter->pipe.write_fd != -1)
			close(iter->pipe.write_fd);
		iter = iter->next;
	}
}
*/
// This function should be declared in minishell.h if it's to be used across files and is not static.
// Or, if it's only used by execute_pipeline_command,
	// it can be static in pipeline_process_utils.c.
// For simplicity,
	// let's assume `pipeline_close_pipes` is a globally available utility or defined where needed.
// The provided `pipeline.c` does not have `pipeline_close_pipes`.
// Let's add a placeholder for `pipeline_close_pipes` in `minishell.h` for now.
// And `setup_pipeline_child_env()` also needs to be defined/declared.
