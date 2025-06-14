/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:52:15 by muiida            #+#    #+#             */
/*   Updated: 2025/06/15 07:15:58 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** 文字列をlong longへ変換
*/
long long	ft_atoll_safe(const char *str, int *overflow);
int			is_numeric_string(const char *str);
void		put_exit_error(const char *msg, const char *arg);

static long long	ft_atoll(const char *str)
{
	int	overflow;

	return (ft_atoll_safe(str, &overflow));
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
		if (!is_numeric_string(args[1]))
		{
			put_exit_error(": numeric argument required\n", args[1]);
			exit(2);
		}
		n = ft_atoll(args[1]);
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
