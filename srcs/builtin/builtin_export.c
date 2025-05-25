/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/26 00:08:32 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"

// /* ノードをリストに追加する */
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

// 環境変数ノードを作成する
t_env	*create_env_node_from_existing(t_env *original)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->name = ft_strdup(original->name);
	if (!new_node->name)
	{
		free(new_node);
		return (NULL);
	}
	if (original->value)
	{
		new_node->value = ft_strdup(original->value);
		if (!new_node->value)
		{
			free(new_node->name);
			free(new_node);
			return (NULL);
		}
	}
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
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

/* ソートされた環境変数を表示する */
static void	print_sorted_env(void)
{
	t_env	*env_list_copy;
	t_env	*sorted_list_copy;

	env_list_copy = duplicate_env_list(*get_env_val());
	if (!env_list_copy && *get_env_val() != NULL)
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
