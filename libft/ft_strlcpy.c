/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 13:14:30 by tsukuru           #+#    #+#             */
/*   Updated: 2024/06/07 21:37:42 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	src_len;
	size_t	i;

	src_len = 0;
	i = 0;
	while (src[src_len] != '\0')
		src_len++;
	if (dstsize == 0)
		return (src_len);
	if (dstsize > src_len)
		dstsize = src_len + 1;
	while (i < dstsize - 1 && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	if (dstsize > 0)
		dst[i] = '\0';
	return (src_len);
}

// int main() {
//     char dst[20];
//     const char *src = "Hello, World!";
//     size_t ret;

//     // 十分な容量がある場合
//     ret = strlcpy(dst, src, sizeof(dst));
//     printf("dst: %s, ret: %zu\n", dst, ret);

//     // // 容量不足の場合
//     // ret = ft_strlcpy(dst, src, 5);
//     // printf("dst: %s, ret: %zu\n", dst, ret);

//     // // 空文字列の場合
//     // ret = ft_strlcpy(dst, "", sizeof(dst));
//     // printf("dst: %s, ret: %zu\n", dst, ret);

//     // // NULLの場合
//     // ret = ft_strlcpy(NULL, src, sizeof(dst));
//     // printf("ret: %zu\n", ret);

// 	ret = strlcpy(dst, "aasdjj;s;sa", 100);
// 	printf ("%zu", ret);

//     return (0);
// }