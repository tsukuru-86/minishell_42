/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/10 13:04:12 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	newline;
	int	start;

	newline = 1;
	start = 1;
	i = 1;
	if (args[1] && ft_strlen(args[1]) == 2 && ft_strncmp(args[1], "-n", 2) == 0)
	{
		newline = 0;
		start = 2;
		i = 2;
	}
	while (args[i])
	{
		if (i > start)
			ft_putchar_fd(' ', 1);
		ft_putstr_fd(args[i], 1);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
	return (0);
}
