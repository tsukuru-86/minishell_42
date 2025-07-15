/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 05:37:47 by muiida            #+#    #+#             */
/*   Updated: 2025/06/24 00:04:55 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

/* Initialize tokenizer.
If cmd is NULL, allocate a new command structure */
int	init_tokenizer_stat(t_tokenizer_stat *stat, t_command *cmd)
{
	stat->tokens = NULL;
	stat->i_input = 0;
	stat->needs_cmd_free = false;
	if (!cmd)
	{
		stat->cmd = (t_command *)malloc(sizeof(t_command));
		if (!stat->cmd)
			return (0);
		ft_memset(stat->cmd, 0, sizeof(t_command));
		stat->cmd->last_status = 0;
		stat->needs_cmd_free = true;
	}
	else
		stat->cmd = cmd;
	return (1);
}
