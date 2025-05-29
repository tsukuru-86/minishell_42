/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:33:57 by tsukuru           #+#    #+#             */
/*   Updated: 2024/06/17 19:25:04 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}

// #include <stdio.h>
// #include <strings.h>

// void print_bytes(const void *s, size_t n) {
//     const unsigned char *p = s;
//     for (size_t i = 0; i < n; i++) {
//         printf("%02x ", p[i]);
//     }
//     printf("\n");
// }

// int main() {
//     char str[] = "Hello, World!";
//     size_t len = sizeof(str);

//     printf("元の文字列のバイト: ");
//     print_bytes(str, len);

//     ft_bzero(str, len);

//     printf("ft_bzero適用後のバイト: ");
//     print_bytes(str, len);

//     return (0);
// }