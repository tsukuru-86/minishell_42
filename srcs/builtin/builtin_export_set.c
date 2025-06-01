/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_set.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/06/02 03:12:03 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"
#include <unistd.h>

/* Append a new variable to the environment list */
static int	append_env_node(const char *name, const char *value)
{
	t_env	*new_node;
	t_env	*current;
	t_env	**env;

	env = get_env_val();
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
	{
		ft_printf_fd(STDERR_FILENO, "minishell: export: the name is NULL.\n");
		return (1);
	}
	if (ft_strncmp(name, "?", 2) != 0 && !is_valid_identifier(name))
	{
		ft_printf_fd(STDERR_FILENO,
			"minishell: export: `%s': not a valid identifier.\n", name);
		return (1);
	}
	node = get_env_node(name);
	if (!node)
		return (append_env_node(name, value));
	return (update_env_value(node, value));
}

/* 環境変数リストを表示する
TODO: declare -x は何？
*/
void	print_env_list(t_env *head)
{
	while (head != NULL)
	{
		if (ft_strncmp(head->name, "?", 2) != 0)
		{
			ft_printf_fd(STDOUT_FILENO, "declare -x %s", head->name);
			if (head->value)
			{
				ft_printf_fd(STDOUT_FILENO, "=\"%s\"", head->value);
			}
			ft_printf_fd(STDOUT_FILENO, "\n");
		}
		head = head->next;
	}
}
