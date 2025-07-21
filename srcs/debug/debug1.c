/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 00:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/16 04:35:07 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dbg_printf(const char *format, ...)
{
	va_list	args;
	size_t	len;

	if (!DEBUG_ENABLED)
		return (0);
	len = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%' && *(format + 1) != '\0')
		{
			format++;
			len += parse_and_call_fd(args, format, STDERR_FILENO);
		}
		else
		{
			len += ft_putchar_fd_cnt(*format, STDERR_FILENO);
		}
		format++;
	}
	va_end(args);
	return (len);
}
