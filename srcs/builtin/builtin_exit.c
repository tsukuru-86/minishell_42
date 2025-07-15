/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:52:15 by muiida            #+#    #+#             */
/*   Updated: 2025/07/15 17:34:20 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"
#include <stdlib.h>

/*
** Validation and conversion of numeric arguments (split version)
*/
int	validate_and_convert(char *arg, long long *n)
{
	if (validate_numeric_arg(arg))
	{
		put_exit_error("numeric argument required\n", arg);
		return (2);
	}
	if (convert_numeric_arg(arg, n))
	{
		put_exit_error("numeric argument required\n", arg);
		return (2);
	}
	debug_print_with_int("converted n=", *n);
	return (0);
}

/*
** Argument processing and execution of exit
*/
static int	check_exit_arg_count(int arg_count)
{
	if (arg_count > 2)
	{
		put_exit_error("too many arguments\n", NULL);
		return (1);
	}
	return (0);
}

static void	exec_exit_with_arg(char *arg)
{
	long long	n;
	int			status;
	int			ret;

	debug_print_with_str("process_exit_args arg1=", arg);
	ret = validate_and_convert(arg, &n);
	debug_print_with_int("validate_and_convert ret=", ret);
	if (ret != 0)
		exit(2);
	status = (int)(n % 256);
	debug_print_with_int("exit status before mod=", status);
	if (status < 0)
		status += 256;
	debug_print_with_int("exit status final=", status);
	exit(status);
}

static int	process_exit_args(char **args, int arg_count)
{
	int	ret;

	debug_print_with_int("process_exit_args arg_count=", arg_count);
	if (args[1] && validate_and_convert(args[1], &(long long){0}) == 2)
		exit(2);
	ret = check_exit_arg_count(arg_count);
	if (ret != 0)
		return (ret);
	if (args[1])
		exec_exit_with_arg(args[1]);
	debug_print("exit with status get_exit_status()");
	exit(get_exit_status());
	return (0);
}

/*
** Main body of the exit command
*/
int	builtin_exit(char **args)
{
	int	arg_count;
	int	ret;

	if (isatty(STDIN_FILENO))
		write(1, "exit\n", 5);
	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	ret = process_exit_args(args, arg_count);
	return (ret);
}
