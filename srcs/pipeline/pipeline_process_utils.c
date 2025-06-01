/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_process_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/06/02 03:52:09 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipeline.h"

/* Redirect stdin and stdout based on pipeline pipes */
static void	pipeline_redirect_io(t_command *current)
{
	if (current->pipe.read_fd != -1)
	{
		if (dup2(current->pipe.read_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
	}
	if (current->pipe.write_fd != -1)
	{
		if (dup2(current->pipe.write_fd, STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
	}
}

/* Execute current command within pipeline: handle redirection and run */
static void	pipeline_execute_command_logic(t_command *current)
{
	int	status;
	int	idx;

	if (current->redirects && !setup_redirection(current->redirects))
		exit(EXIT_FAILURE);
	idx = get_builtin_func_idx(current->args[0]);
	if (0 <= idx && idx <= 6)
	{
		status = execute_builtin(current->args);
		if (current->redirects)
			restore_redirection(current->redirects);
		exit(status);
	}
	else
	{
		status = execute_external_command(current);
		exit(status);
	}
}

/* Execute a pipeline command in child: set up env, redirect I/O, close pipes,
	then run */
static void	execute_pipeline_command(t_command *cmd, t_command *current)
{
	setup_child_signals();
	pipeline_redirect_io(current);
	pipeline_close_pipes(cmd);
	pipeline_execute_command_logic(current);
}

/* 子プロセスの作成と実行 */
bool	spawn_pipeline_processes(t_command *cmd)
{
	t_command	*current;
	pid_t		pid;

	current = cmd;
	while (current)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			cleanup_pipeline(cmd);
			return (false);
		}
		if (pid == 0)
		{
			execute_pipeline_command(cmd, current);
			exit(false);
		}
		else
			current->pipe.pid = pid;
		current = current->next;
	}
	return (true);
}

