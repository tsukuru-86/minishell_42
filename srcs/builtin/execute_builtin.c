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


t_builtin_func	*get_builtin_funcs(void)
{
	t_builtin_func	*funcs;

	funcs = malloc(sizeof(t_builtin_func) * 8);
	if (!funcs)
	{
		perror("get_builtin_funcs");
		exit(EXIT_FAILURE);
	}
	funcs[0] = builtin_echo;
	funcs[1] = builtin_cd;
	funcs[2] = builtin_pwd;
	funcs[3] = builtin_export;
	funcs[4] = builtin_unset;
	funcs[5] = builtin_env;
	funcs[6] = builtin_exit;
	funcs[7] = NULL;
	return (funcs);
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

