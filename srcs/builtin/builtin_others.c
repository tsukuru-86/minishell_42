/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_others.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/06 08:54:56 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	if (args[1] && ft_strlen(args[1]) == 2 && ft_strncmp(args[1], "-n", 2) == 0)
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

	if (args[2] != NULL)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	if (args[1] == NULL)
		path = getenv("HOME");
	else
		path = args[1];
	if (chdir(path) == -1)
	{
		ft_printf_fd(STDERR_FILENO, "cd:");
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
		perror("getcwd");
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	return (0);
}
