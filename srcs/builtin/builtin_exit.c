/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:52:15 by muiida            #+#    #+#             */
/*   Updated: 2025/06/12 06:07:12 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** 文字列をlong longへ変換
*/
static long long	ft_atoll(const char *str)
{
	long long	res;
	int			sign;
	int			i;

	res = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

/*
** 数値文字列か判定
*/
static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
		i++;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	return (!str[i]);
}

/*
** エラーメッセージ出力補助
*/
static void	put_exit_error(const char *msg, const char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	if (arg)
		ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd((char *)msg, 2);
}

/*
** exitコマンド本体
*/
int	builtin_exit(char **args)
{
	long long	n;
	int			status;

	write(1, "exit\n", 5);
	if (args[1] && args[2])
	{
		put_exit_error("too many arguments\n", NULL);
		return (1);
	}
	if (args[1])
	{
		if (!is_numeric(args[1]))
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
	return (0);
}
