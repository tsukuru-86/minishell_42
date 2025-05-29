/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 13:56:00 by tkomai            #+#    #+#             */
/*   Updated: 2024/06/17 18:36:19 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*d;
	const unsigned char	*s = (const unsigned char *)src;

	if (dst == NULL && src == NULL)
		return (dst);
	d = (unsigned char *)dst;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

// int	main(void)
// {
// 	char	*b;
// 	char	a[4] = {10, 20, 30, 40};

// 	b = (char *)malloc(sizeof(char) * 200);
// 	if (!b)
// 		return (0);
// 	ft_memcpy(b, a, 333555);
// 	printf("%d %d %d %d\n", b[0], b[1], b[2], b[3]);

// 	// printf("%p\n", memcpy(b, a, 123));
// 	// free(b);
// }
