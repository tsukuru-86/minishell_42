/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 09:32:48 by tkomai            #+#    #+#             */
/*   Updated: 2024/06/13 15:59:21 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	get_num_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char	*duplicate(char *str, int len, int n)
{
	unsigned int	num;
	int				is_negative;

	is_negative = n < 0;
	if (is_negative)
		num = (unsigned int)(-(n + 1)) + 1;
	else
		num = n;
	str[len] = '\0';
	while (len > 0)
	{
		len--;
		str[len] = (num % 10) + '0';
		num /= 10;
	}
	if (is_negative)
		str[0] = '-';
	return (str);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*str;

	if (n == 0)
		return (ft_strdup("0"));
	len = get_num_len(n);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str = duplicate(str, len, n);
	return (str);
}

// int main(void)
// {
// 	int a = -199555; //The establishment year of the NEOREX.
// 	char *str = ft_itoa(a);

// 	printf("%s\n", str);
// }