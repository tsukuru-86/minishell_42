/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 10:48:51 by tsukuru           #+#    #+#             */
/*   Updated: 2024/06/10 19:45:47 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*last;

	last = NULL;
	c = (unsigned char)c;
	while (*s)
	{
		if ((unsigned char)*s == c)
		{
			last = s;
		}
		s++;
	}
	if (c == '\0')
	{
		return ((char *)s);
	}
	return ((char *)last);
}

// #include <stdio.h>

// int main(void)
// {
//     char *s ="I want to make the Youtube video.";
//     char *f = ft_strrchr(s, 't');
//     printf("%s\n", f);
// }