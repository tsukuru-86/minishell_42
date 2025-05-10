/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/11 00:08:35 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_path_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	*find_command(char *cmd, char **envp)
{
	char	*path_env;
	char	*path;
	char	*dir;
	char	full_path[PATH_MAX];

	if (!cmd || !envp)
		return (NULL);
	// コマンドが絶対パスまたは相対パスの場合
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	// PATHからコマンドを探す
	path_env = get_path_env(envp);
	if (!path_env)
		return (NULL);
	path = ft_strdup(path_env);
	if (!path)
		return (NULL);
	dir = strtok(path, ":");
	while (dir)
	{
		ft_strlcpy(full_path, dir, PATH_MAX);
		ft_strlcat(full_path, "/", PATH_MAX);
		ft_strlcat(full_path, cmd, PATH_MAX);
		if (access(full_path, X_OK) == 0)
		{
			free(path);
			return (ft_strdup(full_path));
		}
		dir = strtok(NULL, ":");
	}
	free(path);
	return (NULL);
}

int	execute_external_command(char **args, char **envp)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;

	cmd_path = find_command(args[0], envp);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(args[0], 2);
		return (127);
	}
	pid = fork();
	if (pid == 0)
	{
		// 子プロセス
		if (execve(cmd_path, args, envp) == -1)
		{
			perror("execve");
			free(cmd_path);
			exit(1);
		}
	}
	else if (pid < 0)
	{
		perror("fork");
		free(cmd_path);
		return (1);
	}
	// 親プロセス
	waitpid(pid, &status, 0);
	free(cmd_path);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
