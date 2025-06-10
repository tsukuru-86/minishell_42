/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 06:17:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/11 07:36:17 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_export.h"
#include "libft.h"
#include <stdlib.h>

/*
** 文字列の両端クォートを除去
*/
char	*unquote(const char *src)
{
	size_t	len;
	char	*res;

	if (!src)
		return (NULL);
	len = ft_strlen(src);
	if (len >= 2 && ((src[0] == '\'' && src[len - 1] == '\'') || (src[0] == '\"'
				&& src[len - 1] == '\"')))
	{
		res = ft_substr(src, 1, len - 2);
		return (res);
	}
	return (ft_strdup(src));
}

/*
** 文字列の前後スペース除去＋両端クォート除去
*/
char	*trim_and_unquote(const char *src)
{
	char	*trimmed;
	char	*res;

	trimmed = ft_trim_spaces(src);
	if (!trimmed)
		return (NULL);
	res = unquote(trimmed);
	free(trimmed);
	return (res);
}

/*
** name/valueを正規化
*/
void	normalize_export_args(char **name, char **value)
{
	char	*tmp;

	if (name && *name)
	{
		tmp = trim_and_unquote(*name);
		free(*name);
		*name = tmp;
	}
	if (value && *value)
	{
		tmp = trim_and_unquote(*value);
		free(*value);
		*value = tmp;
	}
}

char	*create_new_value(const char *old_value, const char *append_value)
{
	size_t	len1;
	size_t	len2;
	char	*new_val;

	if (old_value)
		len1 = ft_strlen(old_value);
	else
		len1 = 0;
	if (append_value)
		len2 = ft_strlen(append_value);
	else
		len2 = 0;
	new_val = (char *)malloc(len1 + len2 + 1);
	if (!new_val)
		return (NULL);
	if (old_value)
		ft_strlcpy(new_val, old_value, len1 + 1);
	else
		new_val[0] = '\0';
	if (append_value)
		ft_strlcpy(new_val + len1, append_value, len2 + 1);
	return (new_val);
}
