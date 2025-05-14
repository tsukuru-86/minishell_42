/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 06:21:12 by muiida            #+#    #+#             */
/*   Updated: 2025/05/14 07:01:29 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	add_env_nodes(t_env *head, char **envp);

/**
 * @brief 環境変数リストの先頭ノードを初期化します。
 *
 * @param envp 環境変数の配列
 * @return 初期化された先頭ノード、または失敗時はNULL
 */
t_env	*initialize_env_head(char **envp)
{
	t_env	*head;

	head = create_env_node(envp[0]);
	if (!head)
		return (NULL);
	return (head);
}

/**
 * @brief 環境変数ノードのリストに新しいノードを追加します
 *
 * @param head リストの先頭ノード
 * @param node 追加する新しいノード
 * @return 追加に成功したかどうか
 */
int	append_env_node(t_env *head, t_env *node)
{
	t_env	*current;

	if (!head || !node)
		return (0);
	current = head;
	while (current->next)
		current = current->next;
	current->next = node;
	return (1);
}

/**
 * @brief 環境変数リスト全体を解放します。
 *
 * @param env 解放する環境変数リストの先頭ポインタ。
 */
void	free_env_list(t_env *env)
{
	t_env	*temp;

	while (env)
	{
		temp = env->next;
		if (env->name)
			free((void *)env->name);
		if (env->value)
			free((void *)env->value);
		free(env);
		env = temp;
	}
}

/**
 * @brief 環境変数配列から環境変数リストを作成します
 *
 * @param envp 環境変数の配列
 * @return 作成されたリスト、または失敗時はNULL
 */
t_env	*create_env_list(char **envp)
{
	t_env	*head;

	if (!envp || !*envp)
		return (NULL);
	head = initialize_env_head(envp);
	if (!head)
		return (NULL);
	if (!add_env_nodes(head, envp))
		return (NULL);
	return (head);
}

/**
 * @brief 環境変数リストにノードを追加します。
 *
 * @param head リストの先頭ノード
 * @param envp 環境変数の配列
 * @return 追加に成功したかどうか
 */
int	add_env_nodes(t_env *head, char **envp)
{
	t_env	*node;
	int		i;

	i = 1;
	while (envp[i])
	{
		node = create_env_node(envp[i]);
		if (!node || !append_env_node(head, node))
		{
			free_env_list(head);
			return (0);
		}
		i++;
	}
	return (1);
}
