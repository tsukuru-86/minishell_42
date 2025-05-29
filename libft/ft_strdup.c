/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 10:44:50 by tsukuru           #+#    #+#             */
/*   Updated: 2024/06/13 16:06:52 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*space;
	char	*temp;

	space = (char *)malloc(ft_strlen(s1) + 1);
	temp = space;
	if (space == NULL)
	{
		return (NULL);
	}
	while (*s1)
	{
		*temp++ = *s1++;
	}
	*temp = '\0';
	return (space);
}

// int main(void)
// {
//     char *src = "Bonjour le monde.\n";
//     char *test = ft_strdup(src);
//     printf("%s", test);
// }