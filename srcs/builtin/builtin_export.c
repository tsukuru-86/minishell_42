/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/21 18:28:15 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "error/error_messages.h"
#include "minishell.h"

/* export組み込みコマンド */
static int	check_invalid_identifier(char *arg)
{
	if (arg[0] == '=' || (arg[0] == '+' && arg[1] == '='))
	{
		ft_printf_fd(STDERR_FILENO, ERR_EXPORT_INVALID_ID, arg);
		return (1);
	}
	return (0);
}

int	builtin_export(char **args)
{
	int	i;
	int	ret;
	int	has_error;

	if (!args[1])
	{
		print_sorted_env(1);
		return (0);
	}
	i = 1;
	has_error = 0;
	while (args[i])
	{
		if (check_invalid_identifier(args[i]))
		{
			has_error = 1;
			i++;
			continue ;
		}
		ret = process_export_with_reconstruction(args, &i);
		if (ret)
			has_error = 1;
	}
	return (has_error);
}
