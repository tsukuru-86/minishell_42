/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 00:26:24 by muiida            #+#    #+#             */
/*   Updated: 2025/06/11 13:03:07 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "libft.h"
#include "minishell.h"

/* 文字列が有効な数値かどうかをチェック */
int	is_valid_number(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/* 数値文字列を整数に変換（+"100"や-"100"にも対応） */
// static int	is_exitnum_overflow(unsigned long long num, int sign)
// {
// 	if ((sign == 1 && num > 9223372036854775807ULL) || (sign == -1
// 			&& num > 9223372036854775808ULL))
// 		return (1);
// 	return (0);
// }

// static int	parse_exit_digits(const char *str, int *digit_count,
// 		unsigned long long *num)
// {
// 	int	i;

// 	i = 0;
// 	*digit_count = 0;
// 	*num = 0;
// 	while (str[i] && ft_isdigit(str[i]))
// 	{
// 		if (*digit_count > 19)
// 			return (1);
// 		*num = (*num) * 10 + (str[i] - '0');
// 		(*digit_count)++;
// 		i++;
// 	}
// 	return (0);
// }

// int	parse_exit_number(const char *str)
// {
// 	int					sign;
// 	int					i;
// 	int					digit_count;
// 	unsigned long long	num;

// 	sign = 1;
// 	i = 0;
// 	if (str[i] == '+' || str[i] == '-')
// 	{
// 		if (str[i] == '-')
// 			sign = -1;
// 		i++;
// 	}
// 	if (parse_exit_digits(str + i, &digit_count, &num) || digit_count == 0
// 		|| is_exitnum_overflow(num, sign))
// 		return (2);
// 	return ((int)((num * sign) & 0xFF));
// }
