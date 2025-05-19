/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_modify_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/05/23 00:41:40 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 単一の環境変数ノードを解放する */
static void	free_single_env_node(t_env *node)
{
	if (!node)
		return ;
	free(node->name);
	free(node->value);
	free(node);
}

/* Append a new variable to the environment list */
int	append_env_node(const char *name, const char *value)
{
	t_env	*new_node;
	t_env	*current;
	t_env	**env;

	env = g_env();
	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (-1);
	new_node->name = ft_strdup(name);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	if (!*env)
		*env = new_node;
	else
	{
		current = *env;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
	return (0);
}

/* Set or create an environment variable */
int	set_env_node(const char *name, const char *value)
{
	t_env	*node;

	if (!name)
		return (1);
	if (!is_valid_identifier(name))
	{
		ft_putstr_fd((char *)"minishell: export: `", 2);
		ft_putstr_fd((char *)name, 2);
		ft_putstr_fd((char *)"': not a valid identifier\n", 2);
		return (1);
	}
	node = get_env_node(name);
	if (node)
	{
		return (update_env_value(node, value));
	}
	return (append_env_node(name, value));
}

int	remove_env_var(const char *name)
{
	t_env	*current;
	t_env	*prev;
	t_env	**head_ref;

	head_ref = g_env();
	if (!head_ref || !*head_ref || !name)
		return (1);
	current = *head_ref;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->name, name, ft_strlen(current->name)) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*head_ref = current->next;
			free_single_env_node(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}
