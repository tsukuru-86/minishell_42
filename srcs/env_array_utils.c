/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 00:00:00 by your_username     #+#    #+#             */
/*   Updated: 2025/05/15 01:34:01 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	populate_env_entry(char *entry, t_env *env_node)
{
	size_t	name_len;
	size_t	value_len;

	name_len = ft_strlen(env_node->name);
	value_len = ft_strlen(env_node->value);
	ft_strlcpy(entry, env_node->name, name_len + 1);
	entry[name_len] = '=';
	ft_strlcpy(entry + name_len + 1, env_node->value, value_len + 1);
}

void	free_env_array_entries(char **env_array, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

char	*allocate_env_entry(t_env *env_node)
{
	char	*entry;

	entry = (char *)malloc(ft_strlen(env_node->name) + 1
			+ ft_strlen(env_node->value) + 1);
	if (!entry)
	{
		perror("minishell: malloc failed for env_array entry");
		return (NULL);
	}
	populate_env_entry(entry, env_node);
	return (entry);
}
