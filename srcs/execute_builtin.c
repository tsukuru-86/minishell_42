/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 01:50:00 by muiida           #+#    #+#             */
/*   Updated: 2025/05/19 01:50:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 単一の組み込みコマンドを実行する関数 */
int	execute_builtin_with_redirect(t_command *cmd)
{
	int	status;

	status = execute_builtin(cmd->args);
	if (cmd->redirects)
		restore_redirection(cmd->redirects);
	return (status);
}

/* 指定されたコマンドが組み込みコマンドであるかを判定する関数 */
int	is_builtin(char *cmd)
{
	char	*builtins[] = {"echo", "cd", "pwd", "export", "unset", "env",
			"exit", NULL};
	int		i;

	i = 0;
	while (builtins[i])
	{
		if (strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

/* 組み込みコマンドを実行する関数。コマンド名に基づいて適切な関数を呼び出す */
int	execute_builtin(char **args)
{
	if (strcmp(args[0], "echo") == 0)
		return (builtin_echo(args));
	else if (strcmp(args[0], "cd") == 0)
		return (builtin_cd(args));
	else if (strcmp(args[0], "pwd") == 0)
		return (builtin_pwd(args));
	else if (strcmp(args[0], "export") == 0)
		return (builtin_export(args));
	else if (strcmp(args[0], "unset") == 0)
		return (builtin_unset(args));
	else if (strcmp(args[0], "env") == 0)
		return (builtin_env(args));
	else if (strcmp(args[0], "exit") == 0)
		return (builtin_exit(args));
	return (1); // コマンドが見つからない場合
}
