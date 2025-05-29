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

#include "builtin_commands.h"
#include "minishell.h"
#include <stdlib.h>

/* 単一の組み込みコマンドを実行する関数 */
int	execute_builtin_with_redirect(t_command *cmd)
{
	int	status;

	status = execute_builtin(cmd->args);
	if (cmd->redirects)
		restore_redirection(cmd->redirects);
	return (status);
}

/* 環境変数の一覧を表示する機能。
   内部の環境変数リストから値を持つ変数のみを出力する */
int	display_all_env_vars(int fd)
{
	t_env	*current_env;

	current_env = *get_env_val();
	while (current_env)
	{
		if (current_env->value && ft_strncmp(current_env->name, "?", 2) != 0)
		{
			ft_putstr_fd(current_env->name, fd);
			ft_putstr_fd("=", fd);
			ft_putendl_fd(current_env->value, fd);
		}
		current_env = current_env->next;
	}
	return (0);
}

const char	**get_builtin_name(void)
{
	const char	**builtins;

	builtins = malloc(sizeof(char *) * 8);
	if (!builtins)
	{
		perror("get_builtin_name");
		exit(EXIT_FAILURE);
	}
	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = NULL;
	return (builtins);
}

int	is_builtin(char *cmd)
{
	int			i;
	const char	**builtins;

	builtins = get_builtin_name();
	i = 0;
	while (builtins[i])
	{
		if (ft_strncmp(cmd, builtins[i], ft_strlen(builtins[i])) == 0)
		{
			free((builtins));
			return (1);
		}
		i++;
	}
	free((builtins));
	return (0);
}

/* 組み込みコマンドを実行する関数。コマンド名に基づいて適切な関数を呼び出す */
/* Routing function */
int	execute_builtin(char **args)
{
	const char	**builtins;
	int			status;

	builtins = get_builtin_name();
	if (ft_strncmp(args[0], builtins[0], ft_strlen(builtins[0])) == 0)
		status = builtin_echo(args);
	else if (ft_strncmp(args[0], builtins[1], ft_strlen(builtins[1])) == 0)
		status = builtin_cd(args);
	else if (ft_strncmp(args[0], builtins[2], ft_strlen(builtins[2])) == 0)
		status = builtin_pwd(args);
	else if (ft_strncmp(args[0], builtins[3], ft_strlen(builtins[3])) == 0)
		status = builtin_export(args);
	else if (ft_strncmp(args[0], builtins[4], ft_strlen(builtins[4])) == 0)
		status = builtin_unset(args);
	else if (ft_strncmp(args[0], builtins[5], ft_strlen(builtins[5])) == 0)
		status = display_all_env_vars(1);
	else if (ft_strncmp(args[0], builtins[6], ft_strlen(builtins[6])) == 0)
		status = builtin_exit(args);
	else
		status = 1;
	free(builtins);
	return (status);
}

