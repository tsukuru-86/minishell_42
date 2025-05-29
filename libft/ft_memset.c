/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 18:24:57 by tkomai            #+#    #+#             */
/*   Updated: 2024/04/23 18:52:57 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*p;
	size_t			i;

	p = b;
	i = 0;
	while (i < len)
	{
		p[i] = (unsigned char)c;
		i++;
	}
	return (b);
}

// int main() {
//     int array[10];
// 	int *test;
//     // memset(array, 0, sizeof(array));
// 	ft_memset(array, 0, sizeof(array));

//     for (int i = 0; i < 10; i++) {
//         printf("%d ", array[i]);
//     }
//     printf("\n");

//     return (0);
// }
