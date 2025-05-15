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

#include "../minishell.h"

/* Create a new environment node from a string */
t_env	*create_env_node(const char *str)
{
	t_env	*new;
	char	*equal_pos;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	equal_pos = ft_strchr(str, '=');
	if (!equal_pos)
	{
		new->name = ft_strdup(str);
		new->value = NULL;
	}
	else
	{
		new->name = ft_substr(str, 0, equal_pos - str);
		new->value = ft_strdup(equal_pos + 1);
	}
	new->next = NULL;
	return (new);
}
