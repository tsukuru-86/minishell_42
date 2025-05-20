/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/20 22:24:50 by muiida            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Made non-static to be callable from external_commands_child_utils.c
void	free_env_array(char **env_array, int count)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	// If count is not reliable or array is NULL-terminated, adjust loop
	while (i < count && env_array[i])
		// Check env_array[i] if count might be off
	{
		free(env_array[i]);
		i++;
	}
	// If it's strictly count based and count can be 0 for empty array
	// while (i < count) free(env_array[i++]);
	free(env_array);
}

/* 環境変数リスト内のノード数をカウントする関数 */
// Made non-static
int	count_env_nodes(t_env *env_list)
{
	t_env	*current;
	int		count;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static char	*create_env_string(t_env *env_node)
{
	char	*result;
	size_t	name_len;
	size_t	value_len;

	if (!env_node || !env_node->name) // Value can be NULL
		return (NULL);
	name_len = ft_strlen(env_node->name);
	value_len = 0;
	if (env_node->value)
		value_len = ft_strlen(env_node->value);
	// Allocate enough memory: name + "=" + value + \0
	result = (char *)malloc(name_len + 1 + value_len + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, env_node->name,ft_strlen(env_node->name));
	result[name_len] = '=';
	if (env_node->value)
		ft_strlcpy(result + name_len + 1, env_node->value,ft_strlen(env_node->value));
	else
		result[name_len + 1] = '\0';
			// Handle NULL value as empty string after '='
	return (result);
}

char	**env_list_to_array(t_env *env_list)
{
	t_env	*current;
	char	**env_array;
	int		count;
	int		i;

	count = count_env_nodes(env_list); // Uses the non-static version now
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	current = env_list;
	while (current)
	{
		// Only include variables with a name. Some systems might have env vars without values.
		// The original create_env_string handles env_node->value being NULL.
		if (current->name) // Ensure there's a name
		{
			env_array[i] = create_env_string(current);
			if (!env_array[i])
			{
				free_env_array(env_array, i); // Uses the non-static version
				return (NULL);
			}
			i++;
		}
		current = current->next;
	}
	env_array[i] = NULL; // Null-terminate the array
	return (env_array);
}
