/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 20:39:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/19 01:29:39 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

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
	t_env	*env;

	env = *g_env();
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
	*g_env() = NULL;
}

/* 与えられた環境変数配列から環境変数リストを作成する関数。
   すべての環境変数をノードとして連結リストに変換する */
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
			free_env_list();
			return (NULL);
		}
		current = current->next;
		i++;
	}
	current->next = NULL;
	*g_env() = head;
	return (head);
}

/* 指定された名前の環境変数ノードを検索して返す関数。
   見つからない場合はNULLを返す */
t_env	*get_env_node(const char *name)
{
	t_env	*env;

	env = *g_env();
	while (env)
	{
		if (ft_strncmp(env->name, name, PATH_MAX) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

/* Set or create an environment variable */
int	set_env_node(const char *name, const char *value)
{
	t_env	*env;
	t_env	*node;

	env = *g_env();
	if (!env || !name)
		return (-1);
	if (!env)
	{
		env = create_env_list(NULL);
		if (!env)
			return (-1);
	}
	if (!is_valid_identifier(name))
	{
		ft_putstr_fd((char *)"export: '", 2);
		ft_putstr_fd((char *)name, 2);
		ft_putstr_fd((char *)"': not a valid identifier\n", 2);
		return (1);
	}
	node = get_env_node(name);
	if (node)
		return (update_env_value(node, value));
	return (append_env_node(name, value));
}

static void	free_env_node(t_env *node)
{
	free(node->name);
	if (node->value)
		free(node->value);
	free(node);
}

int	remove_env_var(const char *name)
{
	t_env	*current;
	t_env	*prev;
	t_env	**env;

	env = g_env();
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
			free_env_node(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (1);
}
