/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/06 01:30:31 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../error/error_messages.h"
#include "external.h"
#include "minishell.h"

void	launch_child(char *cmd_path, char **args)
{
	char	**env_array;
	int		env_count;

	env_array = env_list_to_array();
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
		env_count = count_env_nodes(*get_env_val());
		free_env_array(env_array, env_count);
		if (errno == ENOENT)
			exit(127);
		if (errno == EACCES)
			exit(126);
		exit(1);
	}
}

/* 外部コマンドを実行する関数。
   コマンドパスを検索し、子プロセスでコマンドを実行する */
static int	handle_child_process(char *cmd_path, char **args, t_command *cmd)
{
	setup_child_signals();
	launch_child(cmd_path, args);
	exit(execute_external_command(cmd));
}

static int	handle_fork_error(char *cmd_path)
{
	perror("minishell: fork");
	free(cmd_path);
	return (1);
}

static int	check_if_directory(char *cmd_path, char *cmd_name)
{
	struct stat	st;

	if (stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_printf_fd(2, "minishell: %s: Is a directory\n", cmd_name);
		free(cmd_path);
		return (126);
	}
	return (0);
}

int	execute_external_command(t_command *cmd)
{
	pid_t	pid;
	char	*cmd_path;
	char	**args;
	int		dir_check;

	args = cmd->args;
	if (!args || !args[0] || ft_strlen(args[0]) == 0)
		return (0);
	cmd_path = find_command_path(args[0]);
	if (!cmd_path)
	{
		ft_printf_fd(2, ERR_COMMAND_NOT_FOUND, args[0]);
		return (127);
	}
	dir_check = check_if_directory(cmd_path, args[0]);
	if (dir_check != 0)
		return (dir_check);
	pid = fork();
	if (pid == -1)
		return (handle_fork_error(cmd_path));
	if (pid == 0)
		handle_child_process(cmd_path, args, cmd);
	return (wait_parent(pid, cmd_path));
}
