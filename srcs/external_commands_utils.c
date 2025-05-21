/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/22 01:06:09 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdbool.h>

void	free_env_array(char **env_array, int count)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (i < count && env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

/* 環境変数リスト内のノード数をカウントする関数 */

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

	if (!env_node || !env_node->name)
		return (NULL);
	name_len = ft_strlen(env_node->name);
	value_len = 0;
	if (env_node->value)
		value_len = ft_strlen(env_node->value);
	result = (char *)malloc(name_len + 1 + value_len + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, env_node->name, ft_strlen(env_node->name));
	result[name_len] = '=';
	if (env_node->value)
		ft_strlcpy(result + name_len + 1, env_node->value,
			ft_strlen(env_node->value));
	else
		result[name_len + 1] = '\0';
	return (result);
}

static bool	populate_env_array_from_list(char **env_array, t_env *env_list)
{
	t_env	*current;
	int		i;

	i = 0;
	current = env_list;
	while (current)
	{
		if (current->name)
		{
			env_array[i] = create_env_string(current);
			if (!env_array[i])
			{
				free_env_array(env_array, i);
				return (false);
			}
			i++;
		}
		current = current->next;
	}
	env_array[i] = NULL;
	return (true);
}

/* 環境変数リストを execve で使用できる文字列配列に変換する関数 */
char	**env_list_to_array(t_env *env_list)
{
	char	**env_array;
	int		count;

	count = count_env_nodes(env_list);
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	if (!populate_env_array_from_list(env_array, env_list))
		return (NULL);
	return (env_array);
}
