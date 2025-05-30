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
#include <unistd.h>

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
int	builtin_env(char **args)
{
	t_env	*current_env;

	(void)args;
	current_env = *get_env_val();
	while (current_env)
	{
		if (current_env->value && ft_strncmp(current_env->name, "?", 2) != 0)
		{
			ft_putstr_fd(current_env->name, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putendl_fd(current_env->value, STDOUT_FILENO);
		}
		current_env = current_env->next;
	}
	return (0);
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
	const char		**builtins;
	t_builtin_func	*funcs;
	int				i;
	int				status;

	builtins = get_builtin_name();
	funcs = get_builtin_funcs();
	i = 0;
	status = 1;
	while (builtins[i])
	{
		if (ft_strncmp(args[0], builtins[i], ft_strlen(builtins[i])) == 0)
		{
			status = funcs[i](args);
			break ;
		}
		i++;
	}
	free(builtins);
	free(funcs);
	return (status);
}
