/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/07/24 02:45:44 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"

int	builtin_cd(char **args)
{
	char	*path;
	int		ret;

	if (args[2] != NULL)
	{
		ft_printf_fd(STDERR_FILENO, "cd: too many arguments\n");
		return (1);
	}
	if (args[1] == NULL)
		path = getenv("HOME");
	else
		path = args[1];
	if (path && ft_strcmp(path, "..") == 0)
	{
		ret = chdir(path);
	}
	else
	{
		if (access(path, F_OK) == -1)
		{
			ft_printf_fd(STDERR_FILENO, "cd: %s: No such file or directory\n",
				path);
			return (1);
		}
		ret = chdir(path);
	}
	if (ret == -1)
	{
		ft_printf_fd(STDERR_FILENO, "cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	return (0);
}
