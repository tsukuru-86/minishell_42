/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 09:50:20 by tsukuru           #+#    #+#             */
/*   Updated: 2024/05/23 10:09:24 by tsukuru          ###   ########.fr       */
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