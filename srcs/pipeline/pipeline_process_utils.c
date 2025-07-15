/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_process_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/07/11 05:36:31 by muiida           ###   ########.fr       */
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
	int	idx;

	if (!current)
		exit(127);
	if (!current->args || !current->args[0])
		handle_empty_args(current);
	if (current->redirects && !setup_redirection(current->redirects))
		exit(1);
	idx = get_builtin_func_idx(current->args[0]);
	if (0 <= idx && idx <= 6)
		execute_builtin_command(current);
	else
		exit(execute_external_command(current));
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

/* Create and execute child process */
static void	print_redirect_errors(t_redirect *redirect)
{
	t_redirect	*cur;

	cur = redirect;
	while (cur)
	{
		if (cur->error_msg[0] != '\0')
			write(STDERR_FILENO, cur->error_msg, ft_strlen(cur->error_msg));
		cur = cur->next;
	}
}

bool	spawn_pipeline_processes(t_command *cmd)
{
	t_command	*current;
	pid_t		pid;

	current = cmd;
	while (current)
	{
		pid = fork();
		if (pid == -1)
			return (handle_fork_error(cmd));
		if (pid == 0)
		{
			execute_pipeline_command(cmd, current);
			print_redirect_errors(current->redirects);
			exit(false);
		}
		else
			current->pipe.pid = pid;
		current = current->next;
	}
	return (true);
}
