/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 20:10:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/13 06:14:40 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_ll_overflow(long long res, int sign, char c)
{
	if (sign == 1)
	{
		if (res > 9223372036854775807LL / 10)
			return (1);
		if (res == 9223372036854775807LL / 10 && (c - '0') > 7)
			return (1);
	}
	else
	{
		if (res > 9223372036854775807LL / 10)
			return (1);
		if (res == 9223372036854775807LL / 10 && (c - '0') > 8)
			return (1);
	}
	return (0);
}

int	skip_space_and_sign(char *arg, int *sign)
{
	int	i;

	i = 0;
	*sign = 1;
	while (arg[i] == ' ' || arg[i] == '\t' || arg[i] == '\n' || arg[i] == '\v'
		|| arg[i] == '\f' || arg[i] == '\r')
		i++;
	if (arg[i] == '+' || arg[i] == '-')
	{
		if (arg[i] == '-')
			*sign = -1;
		i++;
	}
	return (i);
}

int	parse_ll_digits(char *arg, int *i, int sign, long long *res)
{
	while (arg[*i] >= '0' && arg[*i] <= '9')
	{
		if (is_ll_overflow(*res, sign, arg[*i]))
			return (1);
		*res = *res * 10 + (arg[*i] - '0');
		(*i)++;
	}
	return (0);
}

int	validate_numeric_arg(char *arg)
{
	int	sign;
	int	i;

	i = skip_space_and_sign(arg, &sign);
	if (arg[i] == '\0')
		return (1);
	return (0);
}

int	convert_numeric_arg(char *arg, long long *n)
{
	int			sign;
	int			i;
	long long	res;

	res = 0;
	i = skip_space_and_sign(arg, &sign);
	if (parse_ll_digits(arg, &i, sign, &res))
		return (1);
	if (arg[i] != '\0')
		return (1);
	if (sign == 1)
		*n = res;
	else
		*n = -res;
	return (0);
}
