/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 07:09:25 by muiida            #+#    #+#             */
/*   Updated: 2025/05/14 07:09:34 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_env_nodes(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

char	*format_env_node_to_string(t_env *node)
{
	char	*name_eq;
	char	*full_string;

	if (!node || !node->name)
		return (ft_strdup("=")); // 不正ノードの扱い
	name_eq = ft_strjoin(node->name, "=");
	if (!name_eq)
		return (NULL);
	if (node->value)
		full_string = ft_strjoin(name_eq, node->value);
	else
		full_string = ft_strjoin(name_eq, ""); // valueがNULLなら空文字
	free(name_eq);
	return (full_string);
}

void	free_array_upto_index(char **array, int index)
{
	int	k;

	k = 0;
	while (k < index)
	{
		if (array[k]) //念のためNULLチェック
			free(array[k]);
		k++;
	}
	free(array);
}
