/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_set.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/05/25 04:29:36 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin_commands.h"

/* Append a new variable to the environment list */
static int	append_env_node(const char *name, const char *value)
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

/* Update the value of an existing environment variable */
static int	update_env_value(t_env *env_node, const char *value)
{
	if (env_node->value)
		free(env_node->value);
	if (value)
		env_node->value = ft_strdup(value);
	else
		env_node->value = NULL;
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

/* 環境変数リストを表示する */
void	print_env_list(t_env *head)
{
	while (head != NULL)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(head->name, 1);
		if (head->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(head->value, 1);
			ft_putchar_fd('\"', 1);
		}
		ft_putchar_fd('\n', 1);
		head = head->next;
	}
}
