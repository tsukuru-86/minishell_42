/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:52:15 by muiida            #+#    #+#             */
/*   Updated: 2025/06/16 05:38:56 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin_commands.h"

/*
** 文字列をlong longへ変換（オーバーフロー検出付き）
*/
static long long	ft_atoll(const char *str, int *overflow)
{
	return (ft_atoll_safe(str, overflow));
}

/*
** 数値引数の検証と変換
*/
static int	validate_and_convert(char *arg, long long *n)
{
	int	overflow;

	if (!is_numeric_string(arg))
	{
		put_exit_error(": numeric argument required\n", arg);
		exit(2);
	}
	*n = ft_atoll(arg, &overflow);
	if (overflow)
	{
		put_exit_error(": numeric argument required\n", arg);
		exit(2);
	}
	return (0);
}

/*
** 引数処理とexit実行
*/
static void	process_exit_args(char **args, int arg_count)
{
	long long	n;
	int			status;

	if (arg_count > 2)
	{
		put_exit_error("too many arguments\n", NULL);
		return ;
	}
	if (args[1])
	{
		validate_and_convert(args[1], &n);
		status = (int)(n % 256);
		if (status < 0)
			status += 256;
		exit(status);
	}
	exit(0);
}

/*
** exitコマンド本体
*/
int	builtin_exit(char **args)
{
	int	arg_count;

	if (isatty(STDIN_FILENO))
		write(1, "exit\n", 5);
	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	if (arg_count > 2)
	{
		put_exit_error("too many arguments\n", NULL);
		return (1);
	}
	process_exit_args(args, arg_count);
	return (0);
}
