/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/18 03:23:56 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Get the PATH environment variable string */
char	*get_path_env(void)
{
	t_env	*path_env;

	path_env = get_env_var(*g_env(), "PATH");
	if (path_env)
		return (path_env->value);
	else
	{
		ft_printf_fd(2, "DEBUG: PATH is not set\n");
		return (NULL);
	}
}

char	*find_command(char *cmd)
{
	char	*path_env;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = get_path_env();
	if (!path_env)
		return (NULL);
	return (search_in_path(path_env, cmd));
}

void	launch_child(char *cmd_path, char **args)
{
	char	**env_array;

	setup_child_signals();
	env_array = env_list_to_array(*g_env());
	if (!env_array)
	{
		perror("env_list_to_array");
		free(cmd_path);
		exit(1);
	}
	if (execve(cmd_path, args, env_array) == -1)
	{
		perror("execve");
		free(cmd_path);
		exit(1);
	}
}

static int	launch_parent(pid_t pid, char *cmd_path)
{
	int	status;

	if (pid < 0)
	{
		perror("fork");
		free(cmd_path);
		return (1);
	}
	// 親プロセスのシグナルハンドラを一時的に変更
	setup_parent_signals();
	waitpid(pid, &status, 0);
	free(cmd_path);
	// 実行終了後、通常のシグナルハンドラに戻す
	init_signal_handlers();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			ft_putstr_fd("\n", STDOUT_FILENO);
			return (130); // SIGINTの標準的な終了コード
		}
		else if (WTERMSIG(status) == SIGQUIT)
		{
			ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
			return (131); // SIGQUITの標準的な終了コード
		}
	}
	return (1);
}

int	execute_external_command(t_command *cmd)
// shellパラメータを追加
{
	pid_t	pid;
	char	*cmd_path;

	printf("DEBUG: Command: %s\n", cmd->args[0]);
	cmd_path = find_command(cmd->args[0]);
	printf("DEBUG: Found path: %s\n", cmd_path);
	if (!cmd_path)
	{
		ft_printf_fd(2, "minishell: %s: command not found\n", cmd->args[0]);
		return (127);
	}
	pid = fork();
	if (pid == 0)
		launch_child(cmd_path, cmd->args);
	return (launch_parent(pid, cmd_path)); // shellを渡す
}
