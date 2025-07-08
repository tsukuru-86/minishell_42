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

int	get_builtin_func_idx(char *cmd)
{
	int			i;
	const char	*builtins[8];

	if (!cmd || ft_strlen(cmd) == 0)
		return (-1);
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
		if (ft_strncmp(cmd, builtins[i], ft_strlen(builtins[i])) == 0
			&& ft_strlen(cmd) == ft_strlen(builtins[i]))
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

t_builtin_func	get_builtin_funcs(char *name)
{
	t_builtin_func	funcs[8];
	int				i;

	i = get_builtin_func_idx(name);
	if (i < 0 || 7 <= i)
		return (NULL);
	funcs[0] = &builtin_echo;
	funcs[1] = &builtin_cd;
	funcs[2] = &builtin_pwd;
	funcs[3] = &builtin_export;
	funcs[4] = &builtin_unset;
	funcs[5] = &builtin_env;
	funcs[6] = &builtin_exit;
	funcs[7] = NULL;
	return (funcs[i]);
}
