/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 00:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/14 01:34:50 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdarg.h>
#include <stdio.h>

void	debug_printf_putstr(const char *str)
{
	if (!str)
		return ;
	write(2, str, ft_strlen(str));
}

void	debug_printf_putnbr(int n)
{
	ft_putnbr_fd(n, 2);
}
