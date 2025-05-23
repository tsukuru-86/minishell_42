/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 01:15:47 by muiida           #+#    #+#             */
/*   Updated: 2025/05/24 00:00:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export_utils.h"

/* Split export argument into name and value */
void	split_export_arg(char *arg, char **name, char **value)
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

/* Validate identifier and set environment variable */
int	validate_and_set_env(char *name, char *value)
{
	if (!is_valid_identifier(name))
	{
		ft_putstr_fd((char *)"export: '", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd((char *)"': not a valid identifier\n", 2);
		return (1);
	}
	if (set_env_node(name, value) != 0)
	{
		ft_putstr_fd((char *)ERR_EXPORT_MEMORY_ALLOC, 2);
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
