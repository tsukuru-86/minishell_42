/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 16:32:53 by muiida            #+#    #+#             */
/*   Updated: 2025/05/14 01:37:44 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

size_t	ft_putstr_fd_len(char *s, int fd)
{
	size_t	len;

	if (s == NULL)
		return (0);
	len = ft_strlen(s);
	return (write(fd, s, len));
}

size_t	ft_putchar_fd_len(char c, int fd)
{
	return (write(fd, &c, 1));
}

size_t	ft_putull_fd_len(unsigned long long num, int fd)
{
	size_t	len;

	len = 0;
	if (num == 0)
	{
		return (ft_putchar_fd_len('0', fd));
	}
	if (num < 10)
		return (ft_putchar_fd_len(num + '0', fd));
	len += ft_putull_fd_len(num / 10, fd);
	len += ft_putull_fd_len(num % 10, fd);
	return (len);
}

size_t	ft_puthex_fd_len(unsigned long long n, int uppercase, int fd)
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
		len += ft_puthex_fd_len(n / 16, uppercase, fd);
	len += ft_putchar_fd_len(base[n % 16], fd);
	return (len);
}

size_t	ft_puthex_addr_fd_len(uintptr_t n, int fd)
{
	size_t	len;
	char	*base;

	base = "0123456789abcdef";
	len = 0;
	if (n >= 16)
		len += ft_puthex_addr_fd_len(n / 16, fd);
	len += ft_putchar_fd_len(base[n % 16], fd);
	return (len);
}
