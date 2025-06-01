/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_print.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:52:51 by muiida            #+#    #+#             */
/*   Updated: 2025/06/02 03:56:42 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"

// 環境変数ノードを作成する
static t_env	*create_env_node_from_existing(t_env *original)
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

/* 環境変数リストのコピーを解放するヘルパー関数 */
static void	free_env_list_copy(t_env *head)
{
	t_env	*current_node;
	t_env	*next_node;

	current_node = head;
	while (current_node)
	{
		next_node = current_node->next;
		free(current_node->name);
		free(current_node->value);
		free(current_node);
		current_node = next_node;
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

/* 環境変数リストを指定された形式で表示する
	* format: 0 = env形式 (NAME=value), 1 = export形式 (declare -x NAME="value")
	*/
static void	print_env_list_format(t_env *head, int format)
{
	while (head != NULL)
	{
		if (ft_strncmp(head->name, "?", 2) != 0)
		{
			if (format == 0)
			{
				ft_putstr_fd(head->name, STDOUT_FILENO);
				ft_putstr_fd("=", STDOUT_FILENO);
				if (head->value)
					ft_putendl_fd(head->value, STDOUT_FILENO);
				else
					ft_putendl_fd("", STDOUT_FILENO);
			}
			else
			{
				ft_printf_fd(STDOUT_FILENO, "declare -x %s", head->name);
				if (head->value)
					ft_printf_fd(STDOUT_FILENO, "=\"%s\"", head->value);
				ft_printf_fd(STDOUT_FILENO, "\n");
			}
		}
		head = head->next;
	}
}

/* 統合された環境変数ソート・表示関数
	* format: 0 = env形式, 1 = export形式
	*/
void	print_sorted_env(int format)
{
	t_env	*env_list_copy;
	t_env	*sorted_list_copy;

	env_list_copy = duplicate_env_list(*get_env_val());
	if (!env_list_copy && *get_env_val() != NULL)
	{
		perror("minishell: malloc error during list duplication");
		return ;
	}
	sorted_list_copy = sort_env_list_copy(env_list_copy);
	print_env_list_format(sorted_list_copy, format);
	free_env_list_copy(sorted_list_copy);
}

/* 環境変数の一覧を表示する機能。
   内部の環境変数リストから値を持つ変数のみを辞書順で出力する */
int	builtin_env(char **args)
{
	(void)args;
	print_sorted_env(0);
	return (0);
}
