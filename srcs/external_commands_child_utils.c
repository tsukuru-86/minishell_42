/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands_child_utils.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/05/22 01:03:24 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	launch_child(char *cmd_path, char **args)
{
	char	**env_array;
	int		env_count;

	env_array = env_list_to_array(*g_env());
	if (!env_array)
	{
		perror("minishell: env_list_to_array failed");
		free(cmd_path);
		exit(127);
	}
	if (execve(cmd_path, args, env_array) == -1)
	{
		perror("minishell: execve");
		free(cmd_path);
		env_count = count_env_nodes(*g_env());
		free_env_array(env_array, env_count);
		if (errno == ENOENT)
			exit(127);
		if (errno == EACCES)
			exit(126);
		exit(1);
	}
}

int	launch_parent(pid_t pid, char *cmd_path)
{
	int	status;
	int	exit_status;

	exit_status = 1;
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
	}
	free(cmd_path);
	return (exit_status);
}
