/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_sort.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 21:35:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/09 02:18:35 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

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
		if (ft_strncmp(current->name, next_node->name, ft_strlen(current->name)
				+ 1) > 0)
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
