/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/22 22:16:27 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "builtin_export.h"
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

static int	process_single_export_arg(char *arg)
{
	char	*name;
	char	*value;
	int		append;
	int		ret;

	debug_print_with_str("[DEBUG] process_single_export_arg() arg: ", arg,
		DEBUG_ENABLED);
	split_export_arg(arg, &name, &value, &append);
	debug_print_with_str("[DEBUG] name=", name, DEBUG_ENABLED);
	debug_print_with_str("[DEBUG] value=", value, DEBUG_ENABLED);
	debug_print_with_int("[DEBUG] append=", append, DEBUG_ENABLED);
	if (append)
	{
		debug_print("[DEBUG] append mode", DEBUG_ENABLED);
		ret = append_env_value(name, value);
	}
	else
	{
		debug_print("[DEBUG] set mode", DEBUG_ENABLED);
		ret = validate_and_set_env(name, value);
	}
	debug_print_with_int("[DEBUG] result: ", ret, DEBUG_ENABLED);
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
