/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_extra.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 02:05:00 by muiida           #+#    #+#             */
/*   Updated: 2025/05/11 02:05:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

/* Create a new environment node from a string */
t_env	*create_env_node(const char *str)
{
	t_env	*new_node;
	char	*equal_pos;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	equal_pos = ft_strchr(str, '=');
	if (!equal_pos)
	{
		new_node->name = ft_strdup(str);
		new_node->value = NULL;
	}
	else
	{
		new_node->name = ft_substr(str, 0, equal_pos - str);
		new_node->value = ft_strdup(equal_pos + 1);
	}
	new_node->next = NULL;
	return (new_node);
}
