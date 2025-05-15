/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_modification_removers.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:40:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/15 08:33:36 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief 単一の環境変数ノードを解放します
 */
void	free_single_env_node(t_env *node)
{
	if (!node)
		return ;
	if (node->name)
		free(node->name);
	if (node->value)
		free(node->value);
	free(node);
}

/**
 * @brief unset コマンドにおける識別子の妥当性チェックとエラー表示を行います。
 * @param name チェックする識別子名。
 * @return 妥当な場合は0、不正な場合は1。
 */
int	check_identifier_for_unset(const char *name)
{
	if (!is_valid_identifier(name))
	{
		ft_printf_fd(STDERR_FILENO,
			"minishell: unset: `%s' is not a valid identifier.\n",
			(char *)name);
		return (1);
	}
	return (0);
}

/**
 * @brief 環境変数リストからノードを取り除きます
 */
static int	remove_node_from_list(t_env **env, t_env *current, t_env *prev)
{
	if (prev)
		prev->next = current->next;
	else
		*env = current->next;
	free_single_env_node(current);
	return (0);
}

/**
 * @brief 環境変数リストから指定された名前の変数を削除します
 */
int	remove_env_var(t_env **env, const char *name)
{
	t_env	*current;
	t_env	*prev;

	if (!env || !*env || !name)
		return (0);
	if (check_identifier_for_unset(name) != 0)
		return (1);
	current = *env;
	prev = NULL;
	while (current)
	{
		if (current->name && ft_strncmp(current->name, name, PATH_MAX) == 0)
			return (remove_node_from_list(env, current, prev));
		prev = current;
		current = current->next;
	}
	return (0);
}
