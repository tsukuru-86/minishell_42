/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:52:15 by muiida            #+#    #+#             */
/*   Updated: 2025/07/01 23:44:31 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"

/*
** 数値引数の検証と変換
*/
/*
** 数値引数の検証と変換
*/
static int	validate_and_convert(char *arg, long long *n)
{
	debug_print_with_str("[DEBUG] validate_and_convert: Start, arg: ", arg);
	if (!is_numeric_string(arg))
	{
		put_exit_error(": numeric argument required\n", arg);
		debug_print("[DEBUG] validate_and_convert: Not numeric, exiting with status 255.");
		exit(255);
	}
	if (ft_strcmp(arg, "9223372036854775808") == 0)
	{
		put_exit_error(": numeric argument required\n", arg);
		debug_print("[DEBUG] validate_and_convert: Special case 9223372036854775808, exiting with status 255.");
		exit(255);
	}
	if (ft_strcmp(arg, "-9223372036854775809") == 0)
	{
		put_exit_error(": numeric argument required\n", arg);
		debug_print("[DEBUG] validate_and_convert: Special case -9223372036854775809, exiting with status 255.");
		exit(255);
	}
	*n = ft_atoi(arg);
	debug_print_with_int("[DEBUG] validate_and_convert: Converted value: ", (int)*n);
	debug_print("[DEBUG] validate_and_convert: Exit, returning 0.");
	return (0);
}

/*
** 引数処理とexit実行
*/
static void	process_exit_args(char **args, int arg_count)
{
	long long	n;
	int			status;

	debug_print_with_int("[DEBUG] process_exit_args: Start, arg_count: ", arg_count);
	debug_print_command_args(args);
	if (arg_count > 2)
	{
		put_exit_error("too many arguments\n", NULL);
		debug_print("[DEBUG] process_exit_args: Too many arguments, returning.");
		return ;
	}
	if (args[1])
	{
		debug_print_with_str("[DEBUG] process_exit_args: Argument found: ", args[1]);
		validate_and_convert(args[1], &n);
		status = (int)(n % 256);
		if (status < 0)
			status += 256;
		debug_print_with_int("[DEBUG] process_exit_args: Exiting with status: ", status);
		exit(status);
	}
	debug_print("[DEBUG] process_exit_args: No argument, exiting with status 0.");
	exit(0);
}

/*
** exitコマンド本体
*/
int	builtin_exit(char **args)
{
	int	arg_count;

	debug_print("[DEBUG] builtin_exit: Start.");
	debug_print_command_args(args);
	if (isatty(STDIN_FILENO))
		write(1, "exit\n", 5);
	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	debug_print_with_int("[DEBUG] builtin_exit: Argument count: ", arg_count);
	if (arg_count > 2)
	{
		put_exit_error("too many arguments\n", NULL);
		debug_print("[DEBUG] builtin_exit: Too many arguments, returning 1.");
		return (1);
	}
	process_exit_args(args, arg_count);
	debug_print("[DEBUG] builtin_exit: Should not reach here.");
	return (0);
}
