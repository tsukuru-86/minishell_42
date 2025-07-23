/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 03:56:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/24 02:08:29 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

/* Get the value of an environment variable */
char	*expand_env_var(const char *name)
{
	t_env	*env_node;
	char	*value;

	if (!name || !*name)
		return (ft_strdup("$"));
	if (ft_strncmp(name, "?", 2) == 0)
		return (ft_itoa(get_exit_status()));
	env_node = get_env_node(name);
	if (!env_node || !env_node->value)
		return (ft_strdup(""));
	value = ft_strdup(env_node->value);
	if (!value)
		return (ft_strdup(""));
	return (value);
}

t_env	*get_env_node(const char *name)
{
	t_env	*env_list_head;
	size_t	name_len;

	if (!name)
		return (NULL);
	name_len = ft_strlen(name);
	env_list_head = *get_env_val(true);
	while (env_list_head)
	{
		if (env_list_head->name && ft_strlen(env_list_head->name) == name_len
			&& ft_strncmp(env_list_head->name, name, name_len) == 0)
			return (env_list_head);
		env_list_head = env_list_head->next;
	}
	return (NULL);
}
