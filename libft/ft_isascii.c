/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 16:43:51 by tkomai            #+#    #+#             */
/*   Updated: 2024/04/18 18:02:28 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
	{
		return (1);
	}
	return (0);
}

// #include <stdio.h>
// #include <ctype.h>

// int main(void)
// {
// 	if(ft_isascii('\''))
// 	{
// 		printf("Perfect\n");
// 	}
// 	if(isascii('\''))
// 	{
// 		printf("Perfect\n");
// 	}
// }
