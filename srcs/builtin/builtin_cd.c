/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/12 04:50:35 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"

int	builtin_cd(char **args)
{
	char	*path;

	if (args[2] != NULL)
	{
		ft_printf_fd(STDERR_FILENO, "cd: too many arguments\n");
		return (1);
	}
	if (args[1] == NULL)
		path = getenv("HOME");
	else
		path = args[1];
	if (chdir(path) == -1)
	{
		ft_printf_fd(STDERR_FILENO, "cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	return (0);
}
