/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 16:32:53 by muiida            #+#    #+#             */
/*   Updated: 2025/05/30 08:52:02 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_fd.h"

size_t	ft_putchar_fd_cnt(char c, int fd)
{
	return (write(fd, &c, 1));
}

size_t	ft_putstr_fd_cnt(char *s, int fd)
{
	return (write(fd, s, ft_strlen(s)));
}

size_t	ft_putull_fd(unsigned long long num, int fd)
{
	size_t	len;

	len = 0;
	if (num == 0)
	{
		return (ft_putchar_fd_cnt('0', fd));
	}
	if (num < 10)
		return (ft_putchar_fd_cnt(num + '0', fd));
	len += ft_putull_fd(num / 10, fd);
	len += ft_putull_fd(num % 10, fd);
	return (len);
}

size_t	ft_puthex_fd(unsigned long long n, int uppercase, int fd)
{
	char	*base;
	size_t	len;

	len = 0;
	if (uppercase)
		base = "0123456789ABCDEF";
	else
		base = "0123456789abcdef";
	len = 0;
	if (n >= 16)
		len += ft_puthex_fd(n / 16, uppercase, fd);
	len += ft_putchar_fd_cnt(base[n % 16], fd);
	return (len);
}
