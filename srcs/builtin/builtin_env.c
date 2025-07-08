/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 02:53:01 by muiida            #+#    #+#             */
/*   Updated: 2025/07/09 02:19:28 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

t_env	*create_env_node_from_existing(t_env *original)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->name = ft_strdup(original->name);
	if (!new_node->name)
	{
		free(new_node);
		return (NULL);
	}
	if (original->value)
	{
		new_node->value = ft_strdup(original->value);
		if (!new_node->value)
		{
			free(new_node->name);
			free(new_node);
			return (NULL);
		}
	}
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

/* 環境変数の一覧を表示する機能 */
int	builtin_env(char **args)
{
	(void)args;
	print_sorted_env(0);
	return (0);
}
