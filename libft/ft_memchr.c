/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 14:48:32 by tsukuru           #+#    #+#             */
/*   Updated: 2024/05/28 21:49:15 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*p = (const unsigned char *)s;
	unsigned char		uc;

	uc = (unsigned char)c;
	while (n-- > 0)
	{
		if (*p == uc)
		{
			return ((void *)p);
		}
		p++;
	}
	return (NULL);
}

// #include <stdio.h>
// int main(void)
// {
//     char array[] = "Hello 42Tokyo";
//     char c = '4';
//     char *result = ft_memchr(array, c, sizeof(array));
//     if (result != NULL)
//     {
//         printf("Found '%c' at position:%ld\n", c, result - array);
//     }else
//     {
//         printf("You couldn't find '%c'.\n",c);
//     }
// }
