/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 18:42:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/19 18:42:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_exit_status(t_command *cmd)
{
	if (!cmd)
		return (0);
	while (cmd->prev)
		cmd = cmd->prev;    // 常に先頭のコマンドを使用
	return (cmd->last_status);
}

void	set_exit_status(t_command *cmd, int status)
{
	if (!cmd)
		return ;
	while (cmd->prev)
		cmd = cmd->prev;    // 常に先頭のコマンドを使用
	cmd->last_status = status;
}