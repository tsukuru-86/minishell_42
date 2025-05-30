/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:04:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/30 08:49:31 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_FD_H
# define FT_PRINTF_FD_H

# include "minishell.h"
# include <stdint.h>
# include <stdarg.h>

size_t	ft_putchar_fd_cnt(char c, int fd);
size_t	ft_putstr_fd_cnt(char *s, int fd);
size_t	ft_putull_fd(unsigned long long num, int fd);
size_t	ft_puthex_fd(unsigned long long n, int uppercase, int fd);
size_t	ft_puthex_addr_fd(uintptr_t n, int fd);
size_t	ft_putptr_fd(void *ptr, int fd);
size_t	ft_putnbr_fd_cnt(int n, int fd);

#endif
