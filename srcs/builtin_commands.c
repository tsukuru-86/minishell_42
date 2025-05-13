/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/11 02:25:30 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>

int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	if (args[1] && ft_strncmp(args[1], "-n", 2) == 0)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
	return (0);
}

int	builtin_cd(char **args)
{
	char	*path;

	if (!args[1])
		path = getenv("HOME");
	else
		path = args[1];
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		perror(path);
		return (1);
	}
	return (0);
}

int	builtin_pwd(char **args)
{
	char	cwd[PATH_MAX];

	(void)args;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		ft_putstr_fd("pwd: error retrieving current directory\n", 2);
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	return (0);
}

int	builtin_exit(char **args)
{
	int	status;

	status = 0;
	if (args[1])
	{
		status = ft_atoi(args[1]);
		if (status < 0 || status > 255)
			status = status % 256;
	}
	exit(status);
	return (status);
}
