/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:52:15 by muiida            #+#    #+#             */
/*   Updated: 2025/06/11 13:07:35 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"

int	parse_exit_number(const char *str)
{
	int					i;
	int					digit;
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
