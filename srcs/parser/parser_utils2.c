/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 05:45:59 by muiida            #+#    #+#             */
/*   Updated: 2025/06/18 23:38:56 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/*
** Free the command argument array
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
** Free the command structure
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
