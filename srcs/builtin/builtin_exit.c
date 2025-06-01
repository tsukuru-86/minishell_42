/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:52:15 by muiida            #+#    #+#             */
/*   Updated: 2025/06/02 04:09:47 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"

/* 特殊ケースの処理 */
static int	handle_special_case(char **args)
{
	char	*combined;
	int		status;

	combined = combine_special_args(args);
	if (!is_valid_number(combined))
	{
		exit_with_numeric_error(combined);
		free(combined);
	}
	status = parse_exit_number(combined);
	free(combined);
	return (status);
}

/* 通常ケースの処理 */
static int	handle_normal_case(char **args)
{
	if (!is_valid_number(args[1]))
		exit_with_numeric_error(args[1]);
	return (parse_exit_number(args[1]));
}

int	builtin_exit(char **args)
{
	int	status;

	if (args[1] && args[2] && !is_special_case(args))
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	if (args[1])
	{
		if (is_special_case(args))
			status = (unsigned char)handle_special_case(args);
		else
			status = (unsigned char)handle_normal_case(args);
	}
	else
		status = 0;
	exit(status);
	return (status);
}
