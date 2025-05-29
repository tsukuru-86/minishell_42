/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 09:38:04 by tsukuru           #+#    #+#             */
/*   Updated: 2024/04/26 09:44:15 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*d;
	const char	*s = (const char *)src;
	size_t		i;

	if (dst == NULL && src == NULL)
		return (NULL);
	d = (char *)dst;
	i = 0;
	if (d < s)
	{
		while (i < len)
		{
			d[i] = s[i];
			i++;
		}
	}
	else
	{
		while (len > 0)
		{
			len--;
			d[len] = s[len];
		}
	}
	return (dst);
}

// int main(void)
// {
//     char a[20] = "abcdefg";
//     char b[20] = {0}; // ヌル文字で初期化
//     char c[20] = {0}; // ヌル文字で初期化
//     ft_memmove(b, a, 14);
//     memmove(c, a, 14);
//     printf("%s\n%s\n", b, c);
// }