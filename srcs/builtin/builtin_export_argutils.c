/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_argutils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:43:09 by muiida            #+#    #+#             */
/*   Updated: 2025/06/22 22:05:19 by muiida           ###   ########.fr       */
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
	char	*var_end;

	if (!plus_pos || !equal_pos || plus_pos >= equal_pos)
		return (0);
	var_end = plus_pos - 1;
	while (var_end >= arg && *var_end == ' ')
		var_end--;
	if (var_end < arg)
		return (0);
	p = plus_pos + 1;
	while (p < equal_pos && *p == ' ')
		p++;
	return (p == equal_pos);
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
	char	*var_end;
	char	*raw_name;

	*append = 0;
	plus_pos = ft_strchr(arg, '+');
	equal_pos = ft_strchr(arg, '=');
	if (is_append_pattern(arg, plus_pos, equal_pos))
	{
		var_end = plus_pos - 1;
		while (var_end >= arg && *var_end == ' ')
			var_end--;
		*name = ft_substr(arg, 0, var_end - arg + 1);
		*value = get_export_value(equal_pos + 1);
		*append = 1;
		debug_print_with_str("[DEBUG] append name: ", *name, DEBUG_ENABLED);
		return ;
	}
	if (equal_pos)
	{
		raw_name = ft_substr(arg, 0, equal_pos - arg);
		debug_print_with_str("[DEBUG] raw name: ", raw_name, DEBUG_ENABLED);
		*name = ft_trim_spaces(raw_name);
		*value = get_export_value(equal_pos + 1);
		debug_print_with_str("[DEBUG] trimmed: ", *name, DEBUG_ENABLED);
		free(raw_name);
		return ;
	}
	*name = ft_trim_spaces(arg);
	*value = NULL;
}
