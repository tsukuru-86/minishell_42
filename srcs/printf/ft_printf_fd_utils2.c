/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:52:26 by muiida            #+#    #+#             */
/*   Updated: 2025/05/30 08:52:29 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_fd.h"

size_t	ft_puthex_addr_fd(uintptr_t n, int fd)
{
	size_t	len;
	char	*base;

	base = "0123456789abcdef";
	len = 0;
	if (n >= 16)
		len += ft_puthex_addr_fd(n / 16, fd);
	len += ft_putchar_fd_cnt(base[n % 16], fd);
	return (len);
}

size_t	ft_putptr_fd(void *ptr, int fd)
{
	int	len;

	len = 0;
	if (ptr == NULL)
	{
		return (ft_putstr_fd_cnt("(nil)", fd));
	}
	len = ft_putstr_fd_cnt("0x", fd);
	len += ft_puthex_addr_fd((uintptr_t)ptr, fd);
	return (len);
}

size_t	ft_putnbr_fd_cnt(int n, int fd)
{
	char	c;
	ssize_t	len;

	len = 0;
	if (n == -2147483648)
	{
		return (write(fd, "-2147483648", 11));
	}
	if (n < 0)
	{
		len += write(fd, "-", 1);
		n = -n;
	}
	if (n > 9)
	{
		ft_putnbr_fd(n / 10, fd);
	}
	c = (n % 10) + '0';
	return (len + write(fd, &c, 1));
}
