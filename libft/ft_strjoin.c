/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 20:11:47 by tsukuru           #+#    #+#             */
/*   Updated: 2024/06/17 19:13:31 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	all_len;
	char	*space;
	char	*temp;

	all_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	space = (char *)malloc(all_len);
	temp = space;
	if (!space)
		return (NULL);
	while (*s1)
	{
		*temp++ = (char)*s1++;
	}
	while (*s2)
	{
		*temp++ = (char)*s2++;
	}
	*temp = '\0';
	return (space);
}

// #include <stdio.h>

// int main(void)
// {
//     char *a = "12345";
//     char *b = "67890";

//     char *résultat = ft_strjoin(a, b);
//     printf("Le total est '%s'\n", résultat);
// }