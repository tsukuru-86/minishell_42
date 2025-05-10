/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 20:39:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/11 00:08:43 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_env	*create_env_node(const char *str)
{
	t_env	*new;
	char	*equal_pos;

	new = (t_env *)malloc(sizeof(t_env));
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

void	free_env_list(t_env *env)
{
	t_env	*temp;

	while (env)
	{
		temp = env->next;
		if (env->name)
			free(env->name);
		if (env->value)
			free(env->value);
		free(env);
		env = temp;
	}
}

t_env	*create_env_list(char **envp)
{
	t_env	*head;
	t_env	*current;
	int		i;

	if (!envp || !envp[0])
		return (NULL);
	head = create_env_node(envp[0]);
	if (!head)
		return (NULL);
	current = head;
	i = 1;
	while (envp[i])
	{
		current->next = create_env_node(envp[i]);
		if (!current->next)
		{
			free_env_list(head);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	return (head);
}

t_env	*get_env_var(t_env *env, const char *name)
{
	while (env)
	{
		if (ft_strncmp(env->name, name, PATH_MAX) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

int	set_env_var(t_env **env, const char *name, const char *value)
{
	t_env	*var;
	t_env	*new;
	t_env	*current;

	var = get_env_var(*env, name);
	if (var)
	{
		if (var->value)
			free(var->value);
		if (value)
			var->value = ft_strdup(value);
		else
			var->value = NULL;
		return (0);
	}
	new = (t_env *)malloc(sizeof(t_env));
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

int	remove_env_var(t_env **env, const char *name)
{
	t_env	*current;
	t_env	*prev;

	if (!env || !*env || !name)
		return (-1);
	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->name, name, PATH_MAX) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->name);
			if (current->value)
				free(current->value);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (-1);
}
