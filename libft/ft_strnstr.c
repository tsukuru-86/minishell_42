/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:58:54 by tsukuru           #+#    #+#             */
/*   Updated: 2024/05/28 19:28:25 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strnstr(const char *haystack, const char *needle, size_t max_len)
{
	size_t	needle_len;
	size_t	i;
	char	*h;
	size_t	k;

	h = (char *)haystack;
	needle_len = 0;
	i = 0;
	while (needle[needle_len])
		needle_len++;
	if (needle_len == 0)
		return (h);
	while (h[i] && (i + needle_len) <= max_len)
	{
		k = 0;
		while (k < needle_len && h[i + k] == needle[k])
			k++;
		if (k == needle_len)
			return (&h[i]);
		i++;
	}
	return (NULL);
}

// #include <stdio.h>

// int main(void)
// {
//     char a[] = "Bonkour comment allez-vous?";
//     char b[] = "comment";
//     char *test = ft_strnstr(a, b, sizeof(a));

//     if (test != NULL)
//     {
//         printf("Found %s.\n", test);
//     }else{
//         printf("Not found.\n");
//     }
// }