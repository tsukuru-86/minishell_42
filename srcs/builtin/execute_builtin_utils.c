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
