/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_set.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/07/09 02:23:59 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "minishell.h"

/* Create a new environment node */
static t_env	*create_env_node_safe(const char *name, const char *value)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->name = ft_strdup(name);
	if (!new_node->name)
	{
		free(new_node);
		return (NULL);
	}
	if (value)
	{
		new_node->value = ft_strdup(value);
		if (!new_node->value)
		{
			free(new_node->name);
			free(new_node);
			return (NULL);
		}
	}
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

/* Append a new variable to the environment list */
static int	append_env_node(const char *name, const char *value)
{
	t_env	*new_node;
	t_env	*current;
	t_env	**env;

	env = get_env_val();
	new_node = create_env_node_safe(name, value);
	if (!new_node)
		return (-1);
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

/* Update the value of an existing environment variable */
static int	update_env_value(t_env *env_node, const char *value)
{
	if (value)
	{
		if (env_node->value)
			free(env_node->value);
		env_node->value = ft_strdup(value);
	}
	return (0);
}

/* Set or create an environment variable */
int	set_env_node(const char *name, const char *value)
{
	t_env	*node;

	if (!name)
	{
		ft_printf_fd(STDERR_FILENO, "minishell: export: the name is NULL.\n");
		return (1);
	}
	if (ft_strncmp(name, "?", 2) != 0 && !is_valid_identifier(name))
	{
		ft_printf_fd(STDERR_FILENO,
			"minishell: export: `%s': not a valid identifier\n", name);
		return (1);
	}
	node = get_env_node(name);
	if (!node)
	{
		if (value != NULL)
			return (append_env_node(name, value));
		return (0);
	}
	if (value != NULL)
		return (update_env_value(node, value));
	return (0);
}

/* Set environment variable without validation (for export internal use) */
int	set_env_node_direct(const char *name, const char *value)
{
	t_env	*node;

	if (!name)
		return (1);
	node = get_env_node(name);
	if (!node)
		return (append_env_node(name, value));
	return (update_env_value(node, value));
}
