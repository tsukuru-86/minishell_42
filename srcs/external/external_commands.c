/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/10 13:55:59 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../error/error_messages.h"
#include "external.h"
#include "minishell.h"

void	launch_child(char *cmd_path, char **args)
{
	char	**env_array;
	int		env_count;
	int		err;
	char	*path_env;
	char	buf[256];
	int		stdin_fd;
	ssize_t	n;

	env_array = env_list_to_array();
	if (!env_array)
	{
		perror("minishell: env_list_to_array failed");
		free(cmd_path);
		exit(127);
	}
	/* --- DEBUG LOG: execve直前の状態 --- */
	stdin_fd = dup(0);
	path_env = getenv("PATH");
	if (args && args[0])
		printf("[DEBUG] execve: argv[0]=%s\n", args[0]);
	else
		printf("[DEBUG] execve: argv[0]=NULL\n");
	printf("[DEBUG] execve: cmd_path=%s\n", cmd_path ? cmd_path : "NULL");
	printf("[DEBUG] execve: PATH=%s\n", path_env ? path_env : "NULL");
	if (stdin_fd != -1)
	{
		n = read(stdin_fd, buf, 255);
		if (n > 0)
		{
			buf[n] = '\0';
			printf("[DEBUG] execve: stdin peek='%s'\n", buf);
		}
		else
		{
			printf("[DEBUG] execve: stdin read failed or empty (n=%zd)\n", n);
		}
		close(stdin_fd);
	}
	else
	{
		printf("[DEBUG] execve: dup(0) failed\n");
	}
	/* --- DEBUG LOG END --- */
	if (execve(cmd_path, args, env_array) == -1)
	{
		err = errno;
		perror("minishell: execve");
		if (cmd_path)
			free(cmd_path);
		if (*get_env_val())
			env_count = count_env_nodes(*get_env_val());
		else
			env_count = 0;
		if (env_array)
			free_env_array(env_array, env_count);
		if (err == ENOENT)
			exit(127);
		if (err == EACCES)
			exit(126);
		exit(1);
	}
}

/* 外部コマンドを実行する関数。
   コマンドパスを検索し、子プロセスでコマンドを実行する */
static int	handle_child_process(char *cmd_path, char **args)
{
	setup_child_signals();
	launch_child(cmd_path, args);
	return (1);
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

	if (stat(cmd_path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			ft_printf_fd(2, "minishell: %s: Is a directory\n", cmd_name);
			return (126);
		}
		return (0);
	}
	else
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
	dir_check = check_if_directory(cmd_path, args[0]);
	if (dir_check != 0)
	{
		pid = fork();
		if (pid == -1)
			return (handle_fork_error(cmd_path));
		if (pid == 0)
			exit(dir_check);
		return (wait_parent(pid, cmd_path));
	}
	pid = fork();
	if (pid == -1)
		return (handle_fork_error(cmd_path));
	if (pid == 0)
		handle_child_process(cmd_path, args);
	return (wait_parent(pid, cmd_path));
}
