/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_new.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/24 21:34:04 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ノードをリストに追加する */
static void	add_node_to_list(t_env **head, t_env **tail, t_env *new_node)
{
	if (!*head)
	{
		*head = new_node;
		*tail = new_node;
	}
	else
	{
		(*tail)->next = new_node;
		*tail = new_node;
	}
}

/* 環境変数リストを複製する */
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
		new_node = create_env_node_from_existing(current_original);
		if (!new_node)
		{
			free_env_list_copy(new_head);
			return (NULL);
		}
		add_node_to_list(&new_head, &new_tail, new_node);
		current_original = current_original->next;
	}
	return (new_head);
}

/* 環境変数リストを表示する */
static void	print_env_list(t_env *head)
{
	while (head != NULL)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(head->name, 1);
		if (head->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(head->value, 1);
			ft_putchar_fd('\"', 1);
		}
		ft_putchar_fd('\n', 1);
		head = head->next;
	}
}

/* ソートされた環境変数を表示する */
static void	print_sorted_env(void)
{
	t_env	*env_list_copy;
	t_env	*sorted_list_copy;

	env_list_copy = duplicate_env_list(*g_env());
	if (!env_list_copy && *g_env() != NULL)
	{
		perror("minishell: export: malloc error during list duplication");
		return ;
	}
	sorted_list_copy = sort_env_list_copy(env_list_copy);
	print_env_list(sorted_list_copy);
	free_env_list_copy(sorted_list_copy);
}

/* export組み込みコマンド */
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
