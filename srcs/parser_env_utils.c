/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_env_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 00:00:00 by your_username     #+#    #+#             */
/*   Updated: 2025/05/14 23:13:26 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*create_env_node2(const char *name, const char *value)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("minishell: malloc failed in create_env_node2");
		return (NULL);
	}
	new_node->name = ft_strdup(name);
	if (!new_node->name)
	{
		free(new_node);
		perror("minishell: ft_strdup failed for name in create_env_node2");
		return (NULL);
	}
	new_node->value = ft_strdup(value);
	if (!new_node->value)
	{
		perror("minishell: ft_strdup failed for value in create_env_node2");
		free(new_node->name);
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}
