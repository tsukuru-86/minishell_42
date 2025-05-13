/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_extra.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 02:05:00 by muiida           #+#    #+#             */
/*   Updated: 2025/05/11 02:05:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Create a new environment node from a string */
t_env	*create_env_node(const char *str)
{
	t_env	*new;
	char	*equal_pos;

	new = malloc(sizeof(t_env));
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

/* Update the value of an existing environment variable */
int	update_env_value(t_env *var, const char *value)
{
	if (var->value)
		free(var->value);
	if (value)
		var->value = ft_strdup(value);
	else
		var->value = NULL;
	return (0);
}

/* Append a new variable to the environment list */
int	append_env_var(t_env **env, const char *name, const char *value)
{
	t_env	*new;
	t_env	*current;

	new = malloc(sizeof(t_env));
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

int	remove_env_var(t_env **env,
		const char *name)
{
	t_env	*current;
	t_env	*prev;

	if (!env || !name)
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
	if (!env || !name)
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

/* Convert t_env list to char** array for execve */
char	**env_list_to_array(t_env *env)
{
	t_env	*current;
	char	**env_array;
	int		count;
	int		i;
	char	*tmp;

	count = 0;
	current = env;
	while (current && ++count)
		current = current->next;
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		tmp = ft_strjoin(current->name, "=");
		if (!tmp)
		{
			while (i > 0)
				free(env_array[--i]);
			free(env_array);
			return (NULL);
		}
		env_array[i] = ft_strjoin(tmp, current->value ? current->value : "");
		free(tmp);
		if (!env_array[i])
		{
			while (i > 0)
				free(env_array[--i]);
			free(env_array);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}
