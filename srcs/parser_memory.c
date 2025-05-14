/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_memory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:10:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/14 03:10:36 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// メモリ管理関連の関数を移動

/**
 * @brief コマンドを解放します
 */
void	free_command(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->redirects)
	{
		if (cmd->redirects->file)
			free(cmd->redirects->file);
		free(cmd->redirects);
	}
	free(cmd);
}

/**
 * @brief コマンド配列を解放します
 */
void	free_commands(t_command **cmds, int count)
{
	int	i;

	if (!cmds)
		return ;
	i = 0;
	while (i < count)
	{
		if (cmds[i])
			free_command(cmds[i]);
		i++;
	}
	free(cmds);
}

// その他のメモリ管理関連関数