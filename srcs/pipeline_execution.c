/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:50:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/15 08:36:48 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Redirect stdin and stdout based on pipeline pipes */
void	pipeline_redirect_io(t_command *current)
{
	if (current->pipe.read_fd != -1)
		dup2(current->pipe.read_fd, STDIN_FILENO);
	if (current->pipe.write_fd != -1)
		dup2(current->pipe.write_fd, STDOUT_FILENO);
}

/* Execute current command within pipeline: handle redirection and run */
void	pipeline_execute_command_logic(t_command *current)
{
	if (current->redirects && !setup_redirection(current->redirects))
		exit(EXIT_FAILURE);
	if (is_builtin(current->args[0]))
		execute_builtin(current);
	else
		execute_external_command(current);
}

/* Execute a pipeline command in child: set up environment, */
/* redirect I/O, close pipes, and then run the command. */
// void	execute_pipeline_command(t_command *cmd, t_command *current)
// {
// 	signal(SIGINT, SIG_DFL);
// 	signal(SIGQUIT, SIG_DFL);
// 	// putenv("MINISHELL_PIPELINE=1");//TODO
// 	pipeline_redirect_io(current);
// 	pipeline_close_pipes(cmd);
// 	pipeline_execute_command_logic(current);
// }

/* パイプラインの実行 */
int	execute_pipeline_command(t_command *cmd)
{
	int			status;
	t_command	*current;
	int			pipeline_result;

	pipeline_result = setup_pipeline(cmd);
	if (pipeline_result == 0)
		return (1);
	status = wait_pipeline(cmd);
	current = cmd;
	while (current)
	{
		cleanup_pipeline(current);
		current = current->next;
	}
	return (status);
}
