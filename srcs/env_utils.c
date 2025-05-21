/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 20:39:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/22 00:14:10 by muiida           ###   ########.fr       */
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
	*g_env() = NULL;
}

/* 与えられた環境変数配列から環境変数リストを作成する関数。
   すべての環境変数をノードとして連結リストに変換する */
// create_env_node is assumed to be defined elsewhere or needs to be included.
// For this example, let's assume create_env_node is available.
t_env	*create_env_list(char **envp)
{
	t_env	*head;
	t_env	*current;
	int		i;

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

/* 指定された名前の環境変数ノードを検索して返す関数。
   見つからない場合はNULLを返す */
t_env	*get_env_node(const char *name)
{
	t_env	*env_list_head;

	if (!name)
		return (NULL);
	env_list_head = *g_env();
	while (env_list_head)
	{
		if (ft_strncmp(env_list_head->name, name,
				ft_strlen(env_list_head->name)) == 0)
			return (env_list_head);
		env_list_head = env_list_head->next;
	}
	return (NULL);
}
