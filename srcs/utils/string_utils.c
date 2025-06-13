/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:22:19 by muiida            #+#    #+#             */
/*   Updated: 2025/06/12 20:22:32 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string_utils.h"
#include <stdlib.h>

char	*str_replace_backslash_n(const char *src)
{
	int		i;
	int		j;
	int		len;
	char	*dst;

	i = 0;
	j = 0;
	len = 0;
	while (src[len])
	{
		if (src[len] == '\\' && src[len + 1] == 'n')
			len += 2;
		else
			len++;
	}
	dst = (char *)malloc(len + 1);
	if (!dst)
		return (NULL);
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
	return (dst);
}
