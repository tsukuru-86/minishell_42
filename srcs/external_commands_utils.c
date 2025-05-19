/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/20 03:15:04 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_env_array(char **env_array, int count)
{
	int	i;

	i = 0;
	while (i < count)
		free(env_array[i++]);
	free(env_array);
}

/* 環境変数リスト内のノード数をカウントする関数 */
static int	count_env_nodes(t_env *env_list)
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
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(env_node->name, "=");
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, env_node->value);
	free(tmp);
	return (result);
}

char	**env_list_to_array(t_env *env_list)
{
	t_env	*current;
	char	**env_array;
	int		count;
	int		i;

	count = count_env_nodes(env_list);
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	current = env_list;
	while (current)
	{
		env_array[i] = create_env_string(current);
		if (!env_array[i])
		{
			free_env_array(env_array, i);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}
