/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/11 07:13:11 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../error/error_messages.h"
#include "external.h"
#include "minishell.h"

void	launch_child(char *cmd_path, char **args)
{
	char	**env_array;

	env_array = env_list_to_array();
	if (!env_array)
	{
		perror("minishell: env_list_to_array failed");
		free(cmd_path);
		exit(127);
	}
	execve(cmd_path, args, env_array);
	perror("minishell: execve failed");
	exit(127);
}

/* 外部コマンドを実行する関数。
   コマンドパスを検索し、子プロセスでコマンドを実行する */
int	handle_child_process(char *cmd_path, char **args)
{
	setup_child_signals();
	launch_child(cmd_path, args);
	return (1);
}

int	handle_fork_error(char *cmd_path)
{
	perror("minishell: fork");
	free(cmd_path);
	return (1);
}

int	check_if_directory(char *cmd_path, char *cmd_name)
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
	return (handle_stat_error(cmd_name));
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
