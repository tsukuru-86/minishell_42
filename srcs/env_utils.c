/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid Date        by              +#+  #+#    #+#             */
/*   Updated: 2025/05/13 19:14:29 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	**g_env(void) t_env **g_env(void)
{
	static t_env	*head = NULL;

	return (&head);
}

void	free_env_list(t_env *env)
{
	t_env			*temp;
	static t_env	*head = NULL;

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
	return (&head);
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

t_env	*create_env_list(char **envp) t_env *create_env_list(char **envp)
{
	t_env	*head;
	t_env	*current;
	int		i;
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

t_env	*get_env_var(t_env *env,
		const char *name) t_env *get_env_var(t_env *env, const char *name)
{
	while (env)
	{
		if (ft_strncmp(env->name, name, PATH_MAX) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
	while (env)
	{
		if (ft_strncmp(env->name, name, PATH_MAX) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

/* Set or create an environment variable */
int	set_env_var(t_env **env, const char *name, const char *value)
	/* Set or create an environment variable */
	int set_env_var(t_env **env, const char *name, const char *value)
{
	t_env	*var;
	t_env	*var;

	if (!is_valid_identifier(name))
	{
		ft_putstr_fd("export: '", 2);
		ft_putstr_fd((char *)name, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	var = get_env_var(*env, name);
	if (var)
		return (update_env_value(var, value));
	return (append_env_var(env, name, value));
	if (!is_valid_identifier(name))
	{
		ft_putstr_fd("export: '", 2);
		ft_putstr_fd((char *)name, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	var = get_env_var(*env, name);
	if (var)
		return (update_env_value(var, value));
	return (append_env_var(env, name, value));
}

int	remove_env_var(t_env **env,
		const char *name) int remove_env_var(t_env **env, const char *name)
{
	t_env	*current;
	t_env	*prev;
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
