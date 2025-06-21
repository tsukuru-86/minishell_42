/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 22:55:43 by muiida            #+#    #+#             */
/*   Updated: 2025/06/21 23:47:59 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stddef.h>

char	*ft_strstr(char *str, char *to_find)
{
	int		i;

	while (*str != '\0')
	{
		i = 0;
		while (str[i] == to_find[i] && str[i] != '\0')
		{
			if (str[i] == '\0' && to_find[i] != '\0')
			{
				return (NULL);
			}
			i++;
			if (to_find[i] == '\0')
			{
				return (str);
			}
		}
		str++;
	}
	return (str);
}
