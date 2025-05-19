/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:58:32 by muiida       +#+  #+#    #+#             */
/*   Updated: 2025/05/22 23:12:01 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 環境変数リストのグローバルアクセスポイントを提供する関数。
   静的変数としてリストのヘッドを保持し、シェル全体で共有できるようにする */
t_env	**g_env(void)
{
	static t_env	*head = NULL;

	return (&head);
}

/* 環境変数リスト全体を解放する関数。
   すべてのノードとその名前、値のメモリを適切に解放する */
void	free_env_list(void)
{
	t_env	*temp;
	t_env	*env_head;

	env_head = *g_env();
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
		*g_env() = NULL;
		return (NULL);
	}
	head = create_env_node(envp[0]);
	if (!head)
	{
		*g_env() = NULL;
		return (NULL);
	}
	*g_env() = head;
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
