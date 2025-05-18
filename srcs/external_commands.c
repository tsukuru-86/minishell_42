/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/19 06:04:55 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* コマンドのパスを検索する関数。
   絶対パスまたは相対パスの場合はそのまま、
   単純なコマンド名の場合はPATH環境変数から検索する */
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

/* 外部コマンドを実行する関数。
   コマンドパスを検索し、子プロセスでコマンドを実行する */
int	execute_external_command(char **args)
{
	pid_t	pid;
	char	*cmd_path;

	cmd_path = find_command(args[0]);
	if (!cmd_path)
	{
		ft_putstr_fd((char *)"minishell: command not found: ", 2);
		ft_putendl_fd(args[0], 2);
		return (127);
	}
	pid = fork();
	if (pid == 0)
		launch_child(cmd_path, args);
	return (launch_parent(pid, cmd_path));
}
