/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_argutils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:43:09 by muiida            #+#    #+#             */
/*   Updated: 2025/07/16 04:28:21 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_export.h"
#include "minishell.h"
#include "utils/debug.h"

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

static void	handle_append_pattern(char *arg, char *plus_pos, char *equal_pos,
		t_export_arg *export_arg)
{
	char	*var_end;

	var_end = plus_pos - 1;
	while (var_end >= arg && *var_end == ' ')
		var_end--;
	*(export_arg->name) = ft_substr(arg, 0, var_end - arg + 1);
	*(export_arg->value) = get_export_value(equal_pos + 1);
	*(export_arg->append) = 1;
	dbg_printf("append name: %s", *(export_arg->name));
}

static void	handle_equal_pattern(char *arg, char *equal_pos,
		t_export_arg *export_arg)
{
	char	*raw_name;

	raw_name = ft_substr(arg, 0, equal_pos - arg);
	dbg_printf("raw name: %s", raw_name);
	*(export_arg->name) = ft_trim_spaces(raw_name);
	*(export_arg->value) = get_export_value(equal_pos + 1);
	dbg_printf("trimmed: %s", *(export_arg->name));
	free(raw_name);
}

void	split_export_arg(char *arg, char **name, char **value, int *append)
{
	char			*plus_pos;
	char			*equal_pos;
	t_export_arg	export_arg;

	export_arg.name = name;
	export_arg.value = value;
	export_arg.append = append;
	*append = 0;
	plus_pos = ft_strchr(arg, '+');
	equal_pos = ft_strchr(arg, '=');
	if (is_append_pattern(arg, plus_pos, equal_pos))
	{
		handle_append_pattern(arg, plus_pos, equal_pos, &export_arg);
		return ;
	}
	if (equal_pos)
	{
		handle_equal_pattern(arg, equal_pos, &export_arg);
		return ;
	}
	*name = ft_trim_spaces(arg);
	*value = NULL;
}
