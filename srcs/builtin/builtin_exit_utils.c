/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 00:26:24 by muiida            #+#    #+#             */
/*   Updated: 2025/06/11 13:07:35 by muiida           ###   ########.fr       */
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

/* 数値エラーを処理して終了 */
void	exit_with_numeric_error(const char *arg)
{
	ft_printf_fd(STDERR_FILENO, "exit: %s: numeric argument required\n", arg);
	exit(2);
}

/* 文字が空白か判定 */
int	is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	is_overflow(unsigned long long num, int digit)
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
