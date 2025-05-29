/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 09:23:14 by tsukuru           #+#    #+#             */
/*   Updated: 2024/06/13 16:07:27 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	char	*space;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	space = (char *)malloc(len + 1);
	if (!space)
		return (NULL);
	ft_strlcpy(space, s + start, len + 1);
	return (space);
}

// #include <stdio.h>

// int main(void)
// {
//     char *str = "Bonsoir.";
//     char *dst = ft_substr(str, 2, 3);
//     printf("Le resultat est '%s'\n", dst); // 結果を表示
//     free(dst); // メモリ解放
// }