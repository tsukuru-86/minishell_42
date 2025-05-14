/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_to_array.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 00:00:00 by your_username     #+#    #+#             */
/*   Updated: 2025/05/15 02:35:45 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	fill_env_array(char **env_array, t_env *env, int count)
{
	t_env	*current;
	int		i;

	current = env;
	i = 0;
	while (i < count && current)
	{
		env_array[i] = allocate_env_entry(current);
		if (!env_array[i])
		{
			free_env_array_entries(env_array, i);
			return (0);
		}
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (1);
}

char	**env_list_to_array(t_env *env)
{
	char	**env_array;
	int		count;

	count = count_env_nodes(env);
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
	{
		perror("minishell: malloc failed for env_array");
		return (NULL);
	}
	if (!fill_env_array(env_array, env, count))
		return (NULL);
	return (env_array);
}
