/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:24:45 by tkomai            #+#    #+#             */
/*   Updated: 2025/06/07 03:12:50 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	index;

	index = 0;
	if (!s || !f)
		return ;
	while (s[index])
	{
		f(index, &s[index]);
		index++;
	}
}

// // example_function: converts characters at even indices to uppercase
// void example_function(unsigned int index, char *c)
// {
//     if (index % 2 == 0 && islower((unsigned char)*c))
// Check if the character is lowercase
//         *c = toupper((unsigned char)*c); // Convert to uppercase
// }

// int main(void)
// {
//     char str[] = "hello world";
//     ft_striteri(str, example_function);

//     printf("%s\n", str);
//     return (0);
// }
