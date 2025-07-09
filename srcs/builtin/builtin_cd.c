/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/07/09 02:58:36 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "error/error_messages.h"

int	builtin_cd(char **args)
{
	char	*path;
	int		chdir_result;

	if (args[1] != NULL && args[2] != NULL)
	{
		ft_printf_fd(STDERR_FILENO, "cd: too many arguments\n");
		return (1);
	}
	if (args[1] == NULL)
		path = getenv("HOME");
	else
		path = args[1];
	chdir_result = chdir(path);
	if (chdir_result == -1)
	{
		ft_printf_fd(STDERR_FILENO, "cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	return (0);
}
