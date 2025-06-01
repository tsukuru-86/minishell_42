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

#include "error/error_messages.h"
#include "builtin_commands.h"
#include "minishell.h"

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
static int	validate_and_set_env(char *name, char *value)
{
	if (!is_valid_identifier(name))
	{
		ft_printf_fd(STDERR_FILENO, ERR_EXPORT_INVALID_ID, name);
		return (1);
	}
	if (set_env_node(name, value) != 0)
	{
		ft_printf_fd(STDERR_FILENO, ERR_EXPORT_MALLOC, 2);
		return (1);
	}
	return (0);
}

/* Reconstruct export argument from multiple split arguments */
static char	*reconstruct_split_args(char **args, int start, int *next_idx)
{
	char	*result;
	char	*temp;
	int		i;

	if (!args[start] || !ft_strchr(args[start], '='))
		return (NULL);
	result = ft_strdup(args[start]);
	if (!result)
		return (NULL);
	i = start + 1;
	while (args[i] && !ft_strchr(args[i], '=') && !is_valid_identifier(args[i]))
	{
		temp = ft_strjoin(result, " ");
		free(result);
		if (!temp)
			return (NULL);
		result = ft_strjoin(temp, args[i]);
		free(temp);
		if (!result)
			return (NULL);
		i++;
	}
	*next_idx = i;
	return (result);
}

/* Process a single export argument, returns error code */
static int	process_export_arg(char *arg)
{
	char	*name;
	char	*value;
	int		ret;

	split_export_arg(arg, &name, &value);
	ret = validate_and_set_env(name, value);
	free(name);
	return (ret);
}

/* Process export arguments with reconstruction for split cases */
int	process_export_with_reconstruction(char **args, int *i)
{
	char	*reconstructed;
	int		next_idx;
	int		ret;

	if (ft_strchr(args[*i], '=') && ft_strchr(args[*i], '=')[1] == '\0'
		&& args[*i + 1] && !ft_strchr(args[*i + 1], '='))
	{
		reconstructed = reconstruct_split_args(args, *i, &next_idx);
		if (reconstructed)
		{
			ret = process_export_arg(reconstructed);
			free(reconstructed);
			*i = next_idx;
			return (ret);
		}
	}
	ret = process_export_arg(args[*i]);
	(*i)++;
	return (ret);
}
