/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:52:15 by muiida            #+#    #+#             */
/*   Updated: 2025/06/11 12:30:52 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"

/* 数値エラーを処理して終了 */
static void	exit_with_numeric_error(const char *arg)
{
	ft_printf_fd(STDERR_FILENO, "exit: %s: numeric argument required\n", arg);
	exit(2);
}

/* 文字が空白か判定 */
static int	is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

static int	is_overflow(unsigned long long num, int digit)
{
	if (num > 9223372036854775807ULL / 10)
		return (1);
	if (num == 9223372036854775807ULL / 10)
	{
		if ((unsigned long long)digit > 7)
			return (1);
	}
	return (0);
}

int	parse_exit_number(const char *str)
{
	int	i;
	int	digit;
	unsigned long long	num;

	i = 0;
	num = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		digit = str[i] - '0';
		if (is_overflow(num, digit))
			return (-1);
		num = num * 10 + digit;
		i++;
	}
	while (is_space(str[i]))
		i++;
	if (str[i] != '\0')
		return (-1);
	return ((int)(num % 256));
}

static int	parse_exit_status(const char *str, int *is_num)
{
	int	i;
	int	sign;
	int	status;

	i = 0;
	sign = 1;
	while (is_space(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	status = parse_exit_number(&str[i]);
	if (status == -1)
	{
		*is_num = 0;
		return (0);
	}
	*is_num = 1;
	if (sign == 1)
		return (status);
	return ((256 - status) % 256);
}

int	builtin_exit(char **args)
{
	int	status;
	int	is_num;

	if (args[1] && args[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	status = 0;
	is_num = 1;
	if (args[1])
		status = parse_exit_status(args[1], &is_num);
	if (args[1] && !is_num)
		exit_with_numeric_error(args[1]);
	exit(status);
	return (status);
}
