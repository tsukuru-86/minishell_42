/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:42:31 by muiida            #+#    #+#             */
/*   Updated: 2025/06/10 14:09:25 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_export.h"
#include "minishell.h"
#include "error/error_messages.h"

int	append_env_value(const char *name, const char *value)
{
	t_env	*node;
	char	*new_val;
	size_t	len1;
	size_t	len2;

	node = get_env_node(name);
	if (!node)
		return (set_env_node(name, value));
	if (node->value)
		len1 = ft_strlen(node->value);
	else
		len1 = 0;
	if (value)
		len2 = ft_strlen(value);
	else
		len2 = 0;
	new_val = (char *)malloc(len1 + len2 + 1);
	if (!new_val)
		return (1);
	if (node->value)
		ft_strlcpy(new_val, node->value, len1 + 1);
	else
		new_val[0] = '\0';
	if (value)
		ft_strlcpy(new_val + len1, value, len2 + 1);
	if (node->value)
		free(node->value);
	node->value = new_val;
	return (0);
}

int	validate_and_set_env(char *name, char *value)
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

char	*reconstruct_split_args(char **args, int start, int *next_idx)
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
