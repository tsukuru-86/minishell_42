/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:58:32 by muiida       +#+  #+#    #+#             */
/*   Updated: 2025/07/24 02:52:11 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

/* Provide a global access point to the environment variable list.
   The head is held as a static variable and shared throughout the shell. */
t_env	**get_env_val(bool to_be_kept)
{
	static t_env	*head = NULL;
	t_env			*temp;
	t_env			*next;

	if (!to_be_kept)
	{
		temp = head;
		while (temp)
		{
			next = temp->next;
			if (temp->name)
				free(temp->name);
			if (temp->value)
				free(temp->value);
			free(temp);
			temp = next;
		}
		head = NULL;
	}
	return (&head);
}

/* Free the entire environment variable list.
   Properly free all nodes and their name/value memory. */
void	free_env_list(void)
{
	t_env	*temp;
	t_env	*env_head;

	env_head = *get_env_val(true);
	while (env_head)
	{
		temp = env_head->next;
		if (env_head->name)
			free(env_head->name);
		if (env_head->value)
			free(env_head->value);
		free(env_head);
		env_head = temp;
	}
}

static t_env	*init_env_head(char **envp)
{
	t_env	*head;

	if (!envp || !envp[0])
	{
		*get_env_val(false);
		return (NULL);
	}
	head = create_env_node(envp[0]);
	if (!head)
	{
		*get_env_val(false);
		return (NULL);
	}
	*get_env_val(true) = head;
	return (head);
}

t_env	*create_env_list(char **envp)
{
	t_env	*head;
	t_env	*current;
	int		i;

	head = init_env_head(envp);
	if (!head)
		return (NULL);
	current = head;
	i = 1;
	while (envp[i])
	{
		current->next = create_env_node(envp[i]);
		if (!current->next)
		{
			free_env_list();
			return (NULL);
		}
		current = current->next;
		i++;
	}
	current->next = NULL;
	return (head);
}
