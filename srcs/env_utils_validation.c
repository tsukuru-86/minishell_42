/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_validation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 21:50:25 by muiida            #+#    #+#             */
/*   Updated: 2025/05/14 21:50:26 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	validate_identifier_for_removal(const char *name)
{
	if (check_identifier_for_unset(name) != 0)
		return (1); // 不正な識別子の場合はエラーコード1
	return (0);
}

t_env	*find_and_remove_node(t_env **env, const char *name)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (current->name && ft_strncmp(current->name, name, PATH_MAX) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			return (current);
		}
		prev = current;
		current = current->next;
	}
	return (NULL);
}
