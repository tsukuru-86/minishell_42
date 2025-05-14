/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_modification_setters.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:40:00 by muiida            #+#             */
/*   Updated: 2025/05/14 03:01:39 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief 環境変数リストの末尾に新しい変数を追加します
 */
int	append_env_var(t_env **env_list, const char *name, const char *value)
{
	t_env	*new_node;
	t_env	*current;

	if (!env_list || !name)
		return (1);
	new_node = create_env_node2(name, value);
	if (!new_node)
		return (1);
	if (!*env_list)
	{
		*env_list = new_node;
		return (0);
	}
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
	return (0);
}

int	set_env_var(t_env **env, const char *name, const char *value)
{
	if (!name)
		return (1);
	return (validate_and_update_env(env, name, value));
}
