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

/* Update the value of an existing environment variable */
int	update_env_value(t_env *env_node, const char *value)
{
	if (env_node->value)
		free(env_node->value);
	if (value)
		env_node->value = ft_strdup(value);
	else
		env_node->value = NULL;
	return (0);
}

/* Append a new variable to the environment list */
int	append_env_node(const char *name, const char *value)
{
	t_env	*new;
	t_env	*current;
	t_env	**env;

	env = g_env();
	new = malloc(sizeof(t_env));
	if (!new)
		return (-1);
	new->name = ft_strdup(name);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->next = NULL;
	if (!*env)
		*env = new;
	else
	{
		current = *env;
		while (current->next)
			current = current->next;
		current->next = new;
	}
	return (0);
}
