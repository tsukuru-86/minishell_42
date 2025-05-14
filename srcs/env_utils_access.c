/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_access.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 21:20:37 by muiida            #+#    #+#             */
/*   Updated: 2025/05/14 04:57:23 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	**g_env(void)
{
	static t_env	*head = NULL;

	return (&head);
}

t_env	*get_env_var(t_env *env, const char *name)
{
	if (!env || !name)
		return (NULL);
	while (env)
	{
		if (env->name && ft_strncmp(env->name, name, PATH_MAX) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}
