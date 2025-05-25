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

#include "minishell.h"
#include "builtin_commands.h"

/* 単一の組み込みコマンドを実行する関数 */
int	execute_builtin_with_redirect(t_command *cmd)
{
	int	status;

	status = execute_builtin(cmd->args);
	if (cmd->redirects)
		restore_redirection(cmd->redirects);
	return (status);
}

int	is_builtin(char *cmd)
{
	const char	*builtins[8];
	int			i;

	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = NULL;
	i = 0;
	while (builtins[i])
	{
		if (strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
	i = 0;
	while (builtins[i])
		if (strcmp(cmd, builtins[i++]) == 0)
			return (1);
	return (0);
}

/* 環境変数の一覧を表示する機能。
   内部の環境変数リストから値を持つ変数のみを出力する */
int	display_all_env_vars(int fd)
{
	t_env	*current_env;

	current_env = *get_env_val();
	while (current_env)
	{
		if (current_env->value)
		{
			ft_putstr_fd(current_env->name, fd);
			ft_putstr_fd("=", fd);
			ft_putendl_fd(current_env->value, fd);
		}
		current_env = current_env->next;
	}
	return (0);
}

/* 組み込みコマンドを実行する関数。コマンド名に基づいて適切な関数を呼び出す */
/* Routing function */
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
		return (display_all_env_vars(1));
	else if (strcmp(args[0], "exit") == 0)
		return (builtin_exit(args));
	return (1);
}
