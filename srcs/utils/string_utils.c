/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:22:19 by muiida            #+#    #+#             */
/*   Updated: 2025/06/13 08:43:04 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string_utils.h"
#include <stdlib.h>

static int	count_replaced_length(const char *src)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (src[i])
	{
		if (src[i] == '\\' && src[i + 1] == 'n')
		{
			len++;
			i += 2;
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

static void	fill_replaced_string(const char *src, char *dst)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (src[i])
	{
		if (src[i] == '\\' && src[i + 1] == 'n')
		{
			dst[j++] = '\n';
			i += 2;
		}
		else
			dst[j++] = src[i++];
	}
	dst[j] = '\0';
}

char	*str_replace_backslash_n(const char *src)
{
	int		len;
	char	*dst;

	len = count_replaced_length(src);
	dst = (char *)malloc(len + 1);
	if (!dst)
		return (NULL);
	fill_replaced_string(src, dst);
	return (dst);
}
