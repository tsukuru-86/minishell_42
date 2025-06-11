/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:52:15 by muiida            #+#    #+#             */
/*   Updated: 2025/06/12 05:56:56 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"

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

int	parse_exit_number(const char *str)
{
	int					i;
	int					digit;
	unsigned long long	num;

	i = 0;
	num = 0;
	printf("[parse_exit_number][start] str='%s'\n", str);
	while (str[i] >= '0' && str[i] <= '9')
	{
		digit = str[i] - '0';
		if (is_overflow(num, digit))
		{
			printf("[parse_exit_number][overflow] num=%llu digit=%d\n", num,
				digit);
			return (-1);
		}
		num = num * 10 + digit;
		i++;
	}
	while (is_space(str[i]))
		i++;
	if (str[i] != '\0')
	{
		printf("[parse_exit_number][invalid] str[i]='%c'\n", str[i]);
		return (-1);
	}
	printf("[parse_exit_number][mod256] num=%llu result=%d\n", num, (int)(num
			% 256));
	return ((int)(num % 256));
}

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	mod256(int n)
{
	int	res;

	res = n % 256;
	if (res < 0)
		res += 256;
	return (res);
}

static long long	get_exit_num(const char *str)
{
	return (ft_atoll(str));
}

static int	parse_exit_status(const char *str, int *is_num)
{
	long long	n;

	*is_num = is_numeric(str);
	if (!*is_num)
		return (2);
	n = get_exit_num(str);
	return (mod256(n));
}

int	builtin_exit(char **args)
{
	int	status;
	int	is_num;

	status = 0;
	is_num = 1;
	if (args[1] && args[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	if (args[1])
		status = parse_exit_status(args[1], &is_num);
	if (args[1] && !is_num)
		exit(2);
	exit(status);
	return (status);
}
