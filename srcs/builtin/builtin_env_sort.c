/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_sort.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 21:35:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/04 17:53:36 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"

/* ノードをスワップしてリストの先頭を更新 */
static t_env	*swap_nodes_in_list(t_env *list_head, t_env *prev, t_env *node1,
		t_env *node2)
{
	node1->next = node2->next;
	node2->next = node1;
	if (prev)
		prev->next = node2;
	else
		list_head = node2;
	return (list_head);
}

/* ソートの1回のパスを実行する */
static t_env	*sort_single_pass(t_env *list_head, int *swapped_flag)
{
	t_env	*current;
	t_env	*prev;
	t_env	*next_node;

	*swapped_flag = 0;
	prev = NULL;
	current = list_head;
	while (current && current->next)
	{
		next_node = current->next;
		if (ft_strcmp(current->name, next_node->name) > 0)
		{
			list_head = swap_nodes_in_list(list_head, prev, current, next_node);
			*swapped_flag = 1;
			prev = next_node;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
	return (list_head);
}

/* バブルソートを使用して環境変数リストのコピーをソートする */
t_env	*sort_env_list_copy(t_env *list_head)
{
	int	swapped_in_pass;

	if (!list_head || !list_head->next)
		return (list_head);
	swapped_in_pass = 1;
	while (swapped_in_pass)
	{
		list_head = sort_single_pass(list_head, &swapped_in_pass);
	}
	return (list_head);
}

/* 統合された環境変数ソート・表示関数 */
void	print_sorted_env(int format)
{
	t_env	*env_list_copy;
	t_env	*sorted_list_copy;
	t_env	*current;

	env_list_copy = duplicate_env_list(*get_env_val());
	if (!env_list_copy && *get_env_val() != NULL)
		return (perror("minishell: malloc error during list duplication"));
	sorted_list_copy = sort_env_list_copy(env_list_copy);
	current = sorted_list_copy;
	while (current != NULL)
	{
		if (ft_strncmp(current->name, "?", 2) != 0)
			print_env_format(current, format);
		current = current->next;
	}
	free_env_list_copy(sorted_list_copy);
}
