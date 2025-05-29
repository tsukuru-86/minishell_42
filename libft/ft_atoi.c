/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:20:16 by tkomai            #+#    #+#             */
/*   Updated: 2024/06/13 15:57:27 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

int	overflow(const char *s, long result, int sign)
{
	if (result > LONG_MAX / 10 || (result == LONG_MAX / 10 && (*s
				- '0') > LONG_MAX % 10))
	{
		if (sign == 1)
			return ((int)LONG_MAX);
		else
			return ((int)LONG_MIN);
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	long	result;
	int		sign;
	int		overflow_result;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (*str < '0' || *str > '9')
		return (0);
	while (*str >= '0' && *str <= '9')
	{
		overflow_result = overflow(str, result, sign);
		if (overflow_result != 1)
			return (overflow_result);
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

// int	output(const char *str, int sign)
// {
// 	int	result;
// 	int	digit;

// 	result = 0;
// 	while (*str >= '0' && *str <= '9')
// 	{
// 		digit = *str - '0';
// 		if (sign == 1 && (result > (2147483647 - digit) / 10))
// 			return (2147483647);
// 		if (sign == -1 && (result > (2147483648 - digit) / 10))
// 			return (-2147483648);
// 		result = result * 10 + (*str - '0');
// 		str++;
// 	}
// 	return (result);
// }

// int	ft_atoi(const char *str)
// {
// 	int	result;
// 	int	sign;

// 	sign = 1;
// 	while ((*str >= 9 && *str <= 13) || *str == 32)
// 		str++;
// 	if (*str == '-' || *str == '+')
// 	{
// 		if (*str == '-')
// 			sign = -1;
// 		str++;
// 	}
// 	result = output(str, sign);
// 	return (result * sign);
// }

// int	main(void)
// {
// 	int test;
// 	char *a = "      922337203685477580";

// 	test = ft_atoi(a);

// 	printf("c'est le %d\n", test);
// 	printf("%d\n", atoi("922337203685477580"));
// }
