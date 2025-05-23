/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/24 05:43:43 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 外部コマンドを実行する関数。
   コマンドパスを検索し、子プロセスでコマンドを実行する */
int	execute_external_command(char **args)
{
	pid_t	pid;
	char	*cmd_path;

	if (!args || !args[0])
		return (0);
	cmd_path = find_command_path(args[0]);
	if (!cmd_path)
	{
		ft_putstr_fd((char *)"minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd((char *)": command not found\n", 2);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		free(cmd_path);
		return (1);
	}
	if (pid == 0)
	{
		launch_child(cmd_path, args);
	}
	return (launch_parent(pid, cmd_path));
}
