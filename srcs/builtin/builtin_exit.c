/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:52:15 by muiida            #+#    #+#             */
/*   Updated: 2025/06/10 05:18:38 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"

/* 数値エラーを処理して終了 */
static void	exit_with_numeric_error(const char *arg)
{
	ft_printf_fd(STDERR_FILENO, "exit: %s: numeric argument required\n", arg);
	exit(2);
}

int	builtin_exit(char **args)
{
	int	status;

	if (args[1] && args[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	status = 0;
	if (args[1] && !is_valid_number(args[1]))
		exit_with_numeric_error(args[1]);
	if (args[1])
		status = parse_exit_number(args[1]);
	exit((unsigned int)status);
	return (status);
}
