/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 20:10:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/15 07:17:55 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_overflow(long long res, int digit)
{
	if (res > (9223372036854775807LL - digit) / 10)
		return (1);
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

long long	ft_atoll_safe(const char *str, int *overflow)
{
	long long	res;
	int			sign;
	int			i;

	res = 0;
	*overflow = 0;
	i = skip_whitespace(str);
	sign = parse_sign(str, &i);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (check_overflow(res, str[i] - '0'))
		{
			*overflow = 1;
			return (0);
		}
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}
