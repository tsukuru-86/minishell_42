/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 20:59:56 by muiida            #+#    #+#             */
/*   Updated: 2025/03/16 07:31:19 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	parse_and_call_fd(va_list args, const char *format, int fd)
{
	size_t	len;

	len = 0;
	if (*format == 'c')
		len += ft_putchar_fd_cnt(va_arg(args, int), fd);
	else if (*format == 's')
		len += ft_putstr_fd_cnt(va_arg(args, char *), fd);
	else if (*format == 'p')
		len += ft_putptr_fd(va_arg(args, void *), fd);
	else if (*format == 'd' || *format == 'i')
		len += ft_putnbr_fd_cnt(va_arg(args, int), fd);
	else if (*format == 'u')
		len += ft_putull_fd(va_arg(args, unsigned int), fd);
	else if (*format == 'x')
		len += ft_puthex_fd(va_arg(args, unsigned int), false, fd);
	else if (*format == 'X')
		len += ft_puthex_fd(va_arg(args, unsigned int), true, fd);
	else if (*format == '%')
		len += ft_putchar_fd_cnt('%', fd);
	else
		len += ft_putchar_fd_cnt(*format, fd);
	return (len);
}

size_t	ft_printf(const char *format, ...)
{
	va_list	args;
	size_t	len;

	len = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%' && *(format + 1) != '\0')
		{
			format++;
			len += parse_and_call_fd(args, format, STDOUT_FILENO);
		}
		else
		{
			len += ft_putchar_fd_cnt(*format, STDOUT_FILENO);
		}
		format++;
	}
	va_end(args);
	return (len);
}

int	ft_printf_fd(int fd, const char *format, ...)
{
	va_list	args;
	size_t	len;

	len = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%' && *(format + 1) != '\0')
		{
			format++;
			len += parse_and_call_fd(args, format, fd);
		}
		else
		{
			len += ft_putchar_fd_cnt(*format, fd);
		}
		format++;
	}
	va_end(args);
	return (len);
}
