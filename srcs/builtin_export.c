/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/24 05:43:42 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 環境変数リストのコピーを解放するヘルパー関数
static void	free_env_list_copy(t_env *head)
{
	t_env	*current_node;
	t_env	*next_node;

	current_node = head;
	while (current_node)
	{
		next_node = current_node->next;
		free(current_node->name);
		free(current_node->value); // valueがNULLの場合もfree(NULL)は安全
		free(current_node);
		current_node = next_node;
	}
}

// 環境変数リストを複製するヘルパー関数
// エラー発生時には、それまでに確保したメモリをすべて解放します。
static t_env	*duplicate_env_list(t_env *original_head)
{
	t_env	*new_head;
	t_env	*new_tail;
	t_env	*current_original;
	t_env	*new_node;

	if (!original_head)
		return (NULL);
	new_head = NULL;
	new_tail = NULL;
	current_original = original_head;
	while (current_original)
	{
		new_node = (t_env *)malloc(sizeof(t_env));
		if (!new_node)
		{
			free_env_list_copy(new_head); // 部分的に作成されたリストを解放
			return (NULL);                 // エラー: メモリ割り当て失敗
		}
		new_node->name = ft_strdup(current_original->name);
		if (!new_node->name)
		{
			free(new_node);
			free_env_list_copy(new_head);
			return (NULL); // エラー
		}
		if (current_original->value)
		{
			new_node->value = ft_strdup(current_original->value);
			if (!new_node->value)
			{
				free(new_node->name);
				free(new_node);
				free_env_list_copy(new_head);
				return (NULL); // エラー
			}
		}
		else
			new_node->value = NULL;
		new_node->next = NULL;
		if (!new_head)
		{
			new_head = new_node;
			new_tail = new_node;
		}
		else
		{
			new_tail->next = new_node;
			new_tail = new_node;
		}
		current_original = current_original->next;
	}
	return (new_head);
}

static t_env	*swap_nodes_in_list(t_env *list_head, t_env *prev,
								t_env *node1, t_env *node2)
{
	node1->next = node2->next;
	node2->next = node1;
	if (prev)
		prev->next = node2;
	else
		list_head = node2; // リストの先頭が入れ替わった場合
	return (list_head);
}

// バブルソートを使用して環境変数リストのコピーをソートする
static t_env	*sort_env_list_copy(t_env *list_head)
{
	int		swapped_in_pass;
	t_env	*current;
	t_env	*prev;
	t_env	*next_node;

	if (!list_head || !list_head->next) // 0または1要素のリストはソート不要
		return (list_head);
	swapped_in_pass = 1;
	while (swapped_in_pass) // 前回のパスで交換が行われた場合のみ続行
	{
		swapped_in_pass = 0;
		prev = NULL;
		current = list_head;
		while (current && current->next)
		{
			next_node = current->next;
			// 環境変数名を辞書順で比較
			if (ft_strncmp(current->name, next_node->name, ft_strlen(current->name) + 1) > 0)
			{
				list_head = swap_nodes_in_list(list_head, prev, current,
						next_node);
				swapped_in_pass = 1;
				prev = next_node; // スワップ後、prev はスワップされて左に来たノード (next_node) を指す
				// current はスワップされて右に行ったノードのままなので、次のループで正しく処理される
			}
			else
			{
				prev = current;
				current = current->next;
			}
		}
	}
	return (list_head);
}

static void	print_env_list(t_env *head)
{
	while (head != NULL)
	{
		ft_putstr_fd((char *)"declare -x ", 1);
		ft_putstr_fd(head->name, 1);
		if (head->value)
		{
			ft_putstr_fd((char *)"=\"", 1);
			ft_putstr_fd(head->value, 1);
			ft_putchar_fd('\"', 1);
		}
		ft_putchar_fd('\n', 1);
		head = head->next;
	}
}

static void	print_sorted_env(void)
{
	t_env	*env_list_copy;
	t_env	*sorted_list_copy;

	// 1. グローバル環境変数のコピーを作成
	env_list_copy = duplicate_env_list(*g_env());
	if (!env_list_copy && *g_env() != NULL) // 元のリストは存在するがコピーに失敗した場合
	{
		perror("minishell: export: malloc error during list duplication");
		// TODO: ここで適切なエラー終了ステータスを設定することを検討
		return ;
	}
	// 2. コピーをソート
	sorted_list_copy = sort_env_list_copy(env_list_copy);
	print_env_list(sorted_list_copy);
	// 3. コピーを解放
	free_env_list_copy(sorted_list_copy);
}

int	builtin_export(char **args)
{
	int	i;
	int	ret;

	if (!args[1])
	{
		print_sorted_env();
		return (0);
	}
	i = 1;
	while (args[i])
	{
		ret = process_export_arg(args[i]);
		if (ret)
			return (ret);
		i++;
	}
	return (0);
}
