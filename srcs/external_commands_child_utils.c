/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands_child_utils.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/05/20 22:24:50 by muiida            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	launch_child(char *cmd_path, char **args)
{
	char	**env_array;

	int env_count; // To be used with free_env_array if it needs count
	env_array = env_list_to_array(*g_env()); // from external_commands_utils.c
	if (!env_array)
	{
		perror("minishell: env_list_to_array failed");
		free(cmd_path); // cmd_path was allocated by find_command
		exit(127);      // Or appropriate error code
	}
	if (execve(cmd_path, args, env_array) == -1)
	{
		perror("minishell: execve");
			// Error message includes cmd_path via perror
		free(cmd_path);
		env_count = count_env_nodes(*g_env()); // from external_commands_utils.c
		free_env_array(env_array, env_count);  // from external_commands_utils.c
		// Determine appropriate exit status based on execve error (e.g., ENOENT
		// -> 127, EACCES -> 126)
		if (errno == ENOENT)
			exit(127);
		if (errno == EACCES)
			exit(126);
		exit(1); // General error
	}
	// execve only returns on error,
	// so the following lines in child are not reached on success
}

int	launch_parent(pid_t pid, char *cmd_path)
{
	int status;
	int exit_status;

	exit_status = 1; // Default error
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("minishell: waitpid");
	}
	else
	{
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_status = 128 + WTERMSIG(status);
				// Standard for signaled processes
		// Handle other cases if necessary
	}
	free(cmd_path); // cmd_path was allocated by find_command
	return (exit_status);
}