/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_modification.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 04:41:40 by muiida            #+#    #+#             */
/*   Updated: 2025/05/14 04:41:52 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	remove_env_var(t_env **env, const char *name)
{
	t_env	*node_to_remove;

	if (!env || !*env || !name)
		return (0);
	if (validate_identifier_for_removal(name))
		return (1);
	node_to_remove = find_and_remove_node(env, name);
	if (node_to_remove)
	{
		free_single_env_node(node_to_remove);
		return (0);
	}
	return (0);
}
