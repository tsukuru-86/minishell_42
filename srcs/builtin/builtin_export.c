/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/07/16 04:26:12 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "builtin_export.h"
#include "error/error_messages.h"
#include "minishell.h"
#include "utils/debug.h"

/* export built-in command */
static int	check_invalid_identifier(char *arg)
{
	if (arg[0] == '=' || (arg[0] == '+' && arg[1] == '='))
	{
		ft_printf_fd(STDERR_FILENO, ERR_EXPORT_INVALID_ID, arg);
		return (1);
	}
	return (0);
}

static int	process_single_export_arg(char *arg)
{
	char	*name;
	char	*value;
	int		append;
	int		ret;

	dbg_printf("process_single_export_arg() arg: %s", arg);
	split_export_arg(arg, &name, &value, &append);
	dbg_printf("splited: name=%s value=%s append=%d", name, value, append);
	if (append)
	{
		dbg_printf("append mode");
		ret = append_env_value(name, value);
	}
	else
	{
		dbg_printf("set mode");
		ret = validate_and_set_env(name, value);
	}
	dbg_printf("result: %d", ret);
	free(name);
	free(value);
	return (ret);
}

int	builtin_export(char **args)
{
	int	i;
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
		if (process_single_export_arg(args[i]))
			has_error = 1;
		i++;
	}
	return (has_error);
}
