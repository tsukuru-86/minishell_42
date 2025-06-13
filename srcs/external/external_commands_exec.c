/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands_exec.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 07:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/11 12:11:17 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/error_messages.h"
#include "external.h"
#include "minishell.h"

int	handle_command_not_found(char **args)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (handle_fork_error(NULL));
	if (pid == 0)
	{
		ft_printf_fd(2, ERR_COMMAND_NOT_FOUND, args[0]);
		exit(127);
	}
	return (wait_parent(pid, NULL));
}

int	handle_directory_check(char *cmd_path, char **args)
{
	pid_t	pid;
	int		dir_check;

	dir_check = check_if_directory(cmd_path, args[0]);
	if (dir_check == 0)
		return (0);
	pid = fork();
	if (pid == -1)
		return (handle_fork_error(cmd_path));
	if (pid == 0)
		exit(dir_check);
	return (wait_parent(pid, cmd_path));
}

int	execute_external_main(char *cmd_path, char **args)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (handle_fork_error(cmd_path));
	if (pid == 0)
		handle_child_process(cmd_path, args);
	return (wait_parent(pid, cmd_path));
}

int	handle_stat_error(char *cmd_name)
{
	if (errno == EACCES)
	{
		perror(cmd_name);
		return (126);
	}
	if (errno == ENOENT)
	{
		perror(cmd_name);
		return (127);
	}
	perror(cmd_name);
	return (1);
}

int	execute_external_command(t_command *cmd)
{
	char	*cmd_path;
	char	**args;
	int		result;

	args = cmd->args;
	if (!args || !args[0] || ft_strlen(args[0]) == 0)
		return (0);
	cmd_path = find_command_path(args[0]);
	if (!cmd_path)
		return (handle_command_not_found(args));
	result = handle_directory_check(cmd_path, args);
	if (result != 0)
		return (result);
	return (execute_external_main(cmd_path, args));
}
