/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_ops.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:10:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/14 21:51:30 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// TODO: この処理はどこかに消えたので仮にAIが書いたものを入れた

/**
 * @brief 新しいコマンドを作成します
 */
t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	return (cmd);
}

static int	count_args(char **args)
{
	int	i;

	i = 0;
	if (args)
	{
		while (args[i])
			i++;
	}
	return (i);
}

/**
 * @brief コマンドの引数配列を拡張します
 */
static char	**expand_args_array(char **args, int arg_count)
{
	char	**new_args;
	int		i;

	new_args = (char **)malloc(sizeof(char *) * (arg_count + 2));
	if (!new_args)
		return (NULL);
	i = 0;
	while (i < arg_count)
	{
		new_args[i] = args[i];
		i++;
	}
	return (new_args);
}

/**
 * @brief コマンドの引数を追加します
 */
int	add_argument(t_command *cmd, char *arg)
{
	int		arg_count;
	char	**new_args;
	char	*arg_copy;

	if (!cmd || !arg)
		return (RET_ERROR);
	arg_count = count_args(cmd->args);
	arg_copy = ft_strdup(arg);
	if (!arg_copy)
		return (RET_ERROR);
	new_args = expand_args_array(cmd->args, arg_count);
	if (!new_args)
	{
		free(arg_copy);
		return (RET_ERROR);
	}
	free(cmd->args);
	cmd->args = new_args;
	cmd->args[arg_count] = arg_copy;
	cmd->args[arg_count + 1] = NULL;
	return (RET_SUCCESS);
}

// その他のコマンド操作関連関数