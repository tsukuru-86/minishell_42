/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_argutils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:43:09 by muiida            #+#    #+#             */
/*   Updated: 2025/06/12 17:08:35 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** 文字列の前後スペースを除去
*/
char	*ft_trim_spaces(const char *src)
{
	if (!src)
		return (NULL);
	return (ft_strtrim(src, " \t\n\v\f\r"));
}

char	*strip_quotes(char *str)
{
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if ((str[0] == '\'' && str[len - 1] == '\''))
		return (ft_substr(str, 1, len - 2));
	if ((str[0] == '"' && str[len - 1] == '"'))
		return (ft_substr(str, 1, len - 2));
	return (ft_strdup(str));
}

static int	is_append_pattern(char *arg, char *plus_pos, char *equal_pos)
{
	char	*p;

	if (!plus_pos || !equal_pos || plus_pos + 1 != equal_pos)
		return (0);
	p = plus_pos - 1;
	while (p >= arg)
	{
		if (*p != ' ')
			break ;
		p--;
	}
	if (p < plus_pos - 1)
		return (0);
	return (1);
}

static char	*get_export_value(char *raw_val)
{
	char	*trimmed;
	char	*stripped;

	trimmed = ft_trim_spaces(raw_val);
	stripped = strip_quotes(trimmed);
	free(trimmed);
	return (stripped);
}

void	split_export_arg(char *arg, char **name, char **value, int *append)
{
	char	*plus_pos;
	char	*equal_pos;
	char	*raw_val;

	*append = 0;
	plus_pos = ft_strchr(arg, '+');
	equal_pos = ft_strchr(arg, '=');
	if (is_append_pattern(arg, plus_pos, equal_pos))
	{
		*name = ft_substr(arg, 0, plus_pos - arg);
		raw_val = equal_pos + 1;
		*value = get_export_value(raw_val);
		*append = 1;
	}
	else if (equal_pos)
	{
		*name = ft_substr(arg, 0, equal_pos - arg);
		raw_val = equal_pos + 1;
		*value = get_export_value(raw_val);
	}
	else
	{
		*name = ft_strdup(arg);
		*value = NULL;
	}
}
