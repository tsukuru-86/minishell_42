/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 20:10:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/09 02:27:51 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "minishell.h"

static int	check_overflow(long long res, int digit, int sign)
{
	if (sign > 0)
	{
		if (res > (9223372036854775807LL - digit) / 10)
			return (1);
	}
	else
	{
		if (res > (long long)(9223372036854775808ULL - digit) / 10)
			return (1);
	}
	return (0);
}

static int	skip_whitespace(const char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	return (i);
}

static int	parse_sign(const char *str, int *i)
{
	int	sign;

	sign = 1;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

static int	is_llong_min(const char *str)
{
	const char	*llong_min_str = "9223372036854775808";
	int			i;

	i = 0;
	while (llong_min_str[i] && str[i] == llong_min_str[i])
		i++;
	return (llong_min_str[i] == '\0' && (str[i] < '0' || str[i] > '9'));
}

long long	ft_atoll_safe(const char *str, int *overflow)
{
	long long	res;
	int			sign;
	int			i;

	res = 0;
	*overflow = 0;
	i = skip_whitespace(str);
	sign = parse_sign(str, &i);
	if (sign == -1 && is_llong_min(str + i))
		return (-9223372036854775807LL - 1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (check_overflow(res, str[i] - '0', sign))
		{
			*overflow = 1;
			return (0);
		}
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}
