/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:39:04 by tkomai            #+#    #+#             */
/*   Updated: 2024/04/18 14:57:58 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
	{
		return (1);
	}
	return (0);
}

// #include <stdio.h>
// #include <ctype.h>

// int main(void)
// {
// 	if(ft_isdigit('a'))
// 	{
// 		printf("Perfect\n");
// 	}
// 	if(isdigit('a'))
// 	{
// 		printf("Perfect\n");
// 	}
// }