/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/07/09 02:16:26 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
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

	split_export_arg(arg, &name, &value, &append);
	if (append)
	{
		ret = append_env_value(name, value);
	}
	else
	{
		ret = validate_and_set_env(name, value);
	}
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
