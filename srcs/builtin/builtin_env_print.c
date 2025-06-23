/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_print.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:52:51 by muiida            #+#    #+#             */
/*   Updated: 2025/06/23 22:20:36 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"

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
			return (free_env_list_copy(new_head), NULL);
		if (!new_head)
			new_head = new_node;
		else
			new_tail->next = new_node;
		new_tail = new_node;
		current_original = current_original->next;
	}
	return (new_head);
}

/* 環境変数を形式に応じて表示 */
static void	print_env_format(t_env *env, int format)
{
	if (format == 0)
	{
		if (env->value)
		{
			ft_printf_fd(STDOUT_FILENO, "%s=%s\n", env->name, env->value);
		}
	}
	else
	{
		ft_printf_fd(STDOUT_FILENO, "declare -x %s", env->name);
		if (env->value)
			ft_printf_fd(STDOUT_FILENO, "=\"%s\"", env->value);
		ft_printf_fd(STDOUT_FILENO, "\n");
	}
}

/* 統合された環境変数ソート・表示関数 */
void	print_sorted_env(int format)
{
	t_env	*env_list_copy;
	t_env	*sorted_list_copy;
	t_env	*current;

	env_list_copy = duplicate_env_list(*get_env_val());
	if (!env_list_copy && *get_env_val() != NULL)
	{
		perror("minishell: malloc error during list duplication");
		return ;
	}
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
