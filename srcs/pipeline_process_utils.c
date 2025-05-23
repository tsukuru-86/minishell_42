/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_process_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/05/24 05:50:55 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Redirect stdin and stdout based on pipeline pipes */
static void	pipeline_redirect_io(t_command *current)
{
	if (current->pipe.read_fd != -1)
	{
		if (dup2(current->pipe.read_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 read");
			exit(EXIT_FAILURE);
		}
	}
	if (current->pipe.write_fd != -1)
	{
		if (dup2(current->pipe.write_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 write");
			exit(EXIT_FAILURE);
		}
	}
}

/* Execute current command within pipeline: handle redirection and run */
static void	pipeline_execute_command_logic(t_command *current)
{
	int	status;

	if (current->redirects && !setup_redirection(current->redirects))
		exit(EXIT_FAILURE);
	if (is_builtin(current->args[0]))
	{
		status = execute_builtin(current->args);
		if (current->redirects)
			restore_redirection(current->redirects);
		exit(status);
	}
	else
	{
		status = execute_external_command(current->args);
		exit(status);
	}
}

/* Execute a pipeline command in child: set up env, redirect I/O, close pipes,
	then run */
static void	execute_pipeline_command(t_command *cmd, t_command *current)
{
	setup_child_signals();
	putenv("MINISHELL_PIPELINE=1");
	pipeline_redirect_io(current);
	pipeline_close_pipes(cmd);
	pipeline_execute_command_logic(current);
}

/* 子プロセスの作成と実行 */
int	spawn_pipeline_processes(t_command *cmd)
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
			return (0);
		}
		if (pid == 0)
		{
			execute_pipeline_command(cmd, current);
			exit(EXIT_FAILURE);
		}
		else
			current->pipe.pid = pid;
		current = current->next;
	}
	return (1);
}
