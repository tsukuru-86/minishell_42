/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands_env_arg.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 01:15:47 by muiida           #+#    #+#             */
/*   Updated: 2025/05/11 01:17:55 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Split export argument into name and value */
static void	split_export_arg(char *arg, char **name, char **value)
{
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
	{
		*name = ft_substr(arg, 0, equal_pos - arg);
		*value = equal_pos + 1;
	}
	else
	{
		*name = ft_strdup(arg);
		*value = NULL;
	}
}

/* Validate and set environment variable */
static int	validate_and_set_env(char *name, char *value)
{
	if (set_env_node(name, value) != 0)
	{
		ft_putstr_fd((char *)"export: memory allocation error\n", 2);
		return (1);
	}
	return (0);
}

/* Process a single export argument, returns error code */
int	process_export_arg(char *arg)
{
	char	*name;
	char	*value;
	int		ret;

	split_export_arg(arg, &name, &value);
	ret = validate_and_set_env(name, value);
	free(name);
	return (ret);
}
