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
#include "builtin.h"
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

/* 組み込みコマンドを実行する関数。コマンド名に基づいて適切な関数を呼び出す */
/* Routing function */
int	execute_builtin(char **args)
{
	t_builtin_func	func;
	int				status;

	func = get_builtin_funcs(args[0]);
	if (!func)
		return (EXIT_FAILURE);
	status = func(args);
	return (status);
}
