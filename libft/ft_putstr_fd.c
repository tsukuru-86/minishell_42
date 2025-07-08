/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 09:50:20 by tsukuru           #+#    #+#             */
/*   Updated: 2025/07/09 01:35:54 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}

// int main(void)
// {
//     char *str = "我是日本人\n";
//     ft_putstr_fd(str, 1);
// }

// void ft_putstr_fd(char *s, int fd)
// {
//     while (*s)
//     {
//         write (fd, s, 1);
//         s++;
//     }
// }