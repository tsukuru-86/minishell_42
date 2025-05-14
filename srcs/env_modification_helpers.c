/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_modification_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 21:50:13 by muiida            #+#    #+#             */
/*   Updated: 2025/05/14 21:50:13 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Update the value of an existing environment variable.
 *        古い値は解放され、新しい値が複製されます。
 *        valueがNULLの場合、ノードの値もNULLに設定されます。
 *
 * @param var 更新する t_env ノードへのポインタ。
 * @param value 設定する新しい値。NULLも可。
 * @return 成功時は0、エラー時は1。
 */
int	update_env_value(t_env *env, const char *value)
{
	char	*new_value_dup;

	if (!env)
		return (1);
	if (env->value)
		free(env->value);
	new_value_dup = NULL;
	if (value)
	{
		new_value_dup = ft_strdup(value);
		if (!new_value_dup)
			return (1);
	}
	env->value = new_value_dup;
	return (0);
}

/**
 * @brief 環境変数リストの末尾に新しい変数を追加します。
 *
 * @param env_list 環境変数リストの先頭ポインタのアドレス。
 * @param name 追加する変数の名前。
 * @param value 追加する変数の値。NULLも可。
 * @return 成功時は0、エラー時は1。
 */
int	append_env_var(t_env **env_list, const char *name, const char *value)
{
	t_env	*new;
	t_env	*current;

	if (!env_list || !name) // 基本的なガード
		return (1);
	new = create_new_env_node(name, value);
	if (!new)
		return (1); // ノード作成失敗
	if (!*env_list) // リストが空の場合
	{
		*env_list = new;
	}
	else
	{
		current = *env_list;
		while (current->next)
			current = current->next;
		current->next = new;
	}
	return (0);
}
