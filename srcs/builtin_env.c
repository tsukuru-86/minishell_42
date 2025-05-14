/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/14 22:23:56 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_unset(char **args)
{
	int	i;

	if (!args[1])
	{
		ft_putstr_fd("unset: not enough arguments\n", 2);
		return (1);
	}
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			// ft_putstr_fd("unset: '", 2);
			// ft_putstr_fd(args[i], 2);
			// ft_putstr_fd("': not a valid identifier\n", 2);
			ft_printf_fd(2, "unset: 'cd: %s is not a valid identifier:%s\n",
				args[i], strerror(errno));
			return (1);
		}
		remove_env_var(g_env(), args[i]);
		i++;
	}
	return (0);
}

int	builtin_env(char **args)
{
	extern char	**environ;
	int			i;

	(void)args;
	i = 0;
	while (environ[i])
	{
		ft_putendl_fd(environ[i], 1);
		i++;
	}
	return (0);
}
