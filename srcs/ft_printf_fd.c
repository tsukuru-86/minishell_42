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

#include "../minishell.h"
#include <stdarg.h>

extern size_t	ft_putstr_fd_len(void *ptr, int fd);

size_t	ft_putnbr_fd_len(int n, int fd)
{
	char	c;
	size_t	len;

	len = 0;
	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return (11);
	}
	if (n < 0)
	{
		len += write(fd, "-", 1);
		n = -n;
	}
	if (n > 9)
	{
		len += ft_putnbr_fd_len(n / 10, fd);
	}
	c = (n % 10) + '0';
	write(fd, &c, 1);
	return (len);
}

size_t	ft_putptr_fd_len(void *ptr, int fd)
{
	int	len;

	len = 0;
	if (ptr == NULL)
	{
		return (ft_putstr_fd_len("(nil)", fd));
	}
	len = ft_putstr_fd_len("0x", fd);
	len += ft_puthex_addr_fd_len((uintptr_t)ptr, fd);
	return (len);
}

static size_t	parse_and_call_fd_len(va_list args, const char *format, int fd)
{
	size_t	len;

	len = 0;
	if (*format == 'c')
		len += ft_putchar_fd_len(va_arg(args, int), fd);
	else if (*format == 's')
		len += ft_putstr_fd_len(va_arg(args, char *), fd);
	else if (*format == 'p')
		len += ft_putptr_fd_len(va_arg(args, void *), fd);
	else if (*format == 'd' || *format == 'i')
		len += ft_putnbr_fd_len(va_arg(args, int), fd);
	else if (*format == 'u')
		len += ft_putull_fd_len(va_arg(args, unsigned int), fd);
	else if (*format == 'x')
		len += ft_puthex_fd_len(va_arg(args, unsigned int), false, fd);
	else if (*format == 'X')
		len += ft_puthex_fd_len(va_arg(args, unsigned int), true, fd);
	else if (*format == '%')
		len += ft_putchar_fd_len('%', fd);
	else
		len += ft_putchar_fd_len(*format, fd);
	return (len);
}

size_t	ft_printf_fd(int fd, const char *format, ...)
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
			len += parse_and_call_fd_len(args, format, fd);
		}
		else
		{
			len++;
			ft_putchar_fd_len(*format, fd);
		}
		format++;
	}
	va_end(args);
	return (len);
}
