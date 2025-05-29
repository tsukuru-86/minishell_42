/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 13:41:18 by tkomai            #+#    #+#             */
/*   Updated: 2024/06/10 20:05:40 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	index;
	char			*str;

	index = 0;
	if (!s || !(f))
		return (NULL);
	str = (char *)malloc(ft_strlen(s) + 1);
	if (!str)
		return (NULL);
	while (s[index])
	{
		str[index] = f(index, s[index]);
		index++;
	}
	str[index] = '\0';
	return (str);
}

// char example_function(unsigned int index, char c) {
//     // アルファベットを3文字ずらす
//     if (ft_isalpha(c)) {
//         if (islower(c)) {
//             return (((c - 'a' + 3) % 26) + 'a');
//         } else {
//             return (((c - 'A' + 3) % 26) + 'A');
//         }
//     }
//     return (c);
// }

// int main(void) {
//     char *str = "Hello, World!";
//     char *modified_str = ft_strmapi(str, example_function);

//     if (modified_str) {
//         printf("Modified string: %s\n", modified_str);
//         free(modified_str); // メモリを解放
//     } else {
//         printf("Memory allocation failed\n");
//     }

//     return (0);
// }