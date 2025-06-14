/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/14 19:37:33 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "error/error_messages.h"
#include "minishell.h"

/* エラーメッセージを出力するヘルパー関数 */

static void	print_unset_invalid_identifier_error(const char *identifier)
{
	ft_printf_fd(2, ERR_UNSET_INVALID_ID, identifier);
}

/* 単一の環境変数ノードを解放する */
static void	free_single_env_node(t_env *node)
{
	if (!node)
		return ;
	free(node->name);
	free(node->value);
	free(node);
}

int	remove_env_var(const char *name)
{
	t_env	*current;
	t_env	*prev;
	t_env	**head_ref;

	head_ref = get_env_val();
	if (!head_ref || !*head_ref || !name)
		return (1);
	current = *head_ref;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*head_ref = current->next;
			free_single_env_node(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

/* 単一変数のunset処理 */
static int	process_unset_var(char *arg)
{
	int	is_valid;

	is_valid = is_valid_identifier(arg);
	if (!is_valid)
	{
		print_unset_invalid_identifier_error(arg);
		return (0);
	}
	remove_env_var(arg);
	return (0);
}

/* 環境変数を削除するビルトインコマンド。
   引数チェックを行い、有効な変数名のみ削除を実行する */
int	builtin_unset(char **args)
{
	int	i;
	int	status;

	if (!args[1])
		return (0);
	i = 1;
	status = 0;
	while (args[i])
	{
		if (process_unset_var(args[i]))
			status = 1;
		i++;
	}
	return (status);
}
