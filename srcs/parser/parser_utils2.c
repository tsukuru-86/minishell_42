/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 05:45:59 by muiida            #+#    #+#             */
/*   Updated: 2025/07/09 02:21:14 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
** コマンド引数配列の解放
*/
static void	free_command_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
}

/*
** コマンド構造体の解放
*/
void	free_command(t_command *cmd)
{
	t_command	*next_cmd;

	while (cmd)
	{
		next_cmd = cmd->next;
		if (cmd->args)
			free_command_args(cmd->args);
		if (cmd->redirects)
			free_redirect(cmd->redirects);
		free(cmd);
		cmd = next_cmd;
	}
}
