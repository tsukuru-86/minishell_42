/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 04:07:25 by muiida            #+#    #+#             */
/*   Updated: 2025/05/18 03:25:01 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* duplicate argument vector up to count */
static char	**duplicate_args(char **args, int count)
{
	char	**cmd;
	int		i;

	cmd = (char **)malloc(sizeof(char **) * (count + 1));
	if (!cmd)
		return (NULL);
	i = 0;
	while (i < count)
	{
		cmd[i] = ft_strdup(args[i]);
		if (!cmd[i])
		{
			while (--i >= 0)
				free(cmd[i]);
			free(cmd);
			return (NULL);
		}
		i++;
	}
	cmd[i] = NULL;
	return (cmd);
}

/* prepare command arguments for execution */
char	**prepare_command(char **args, int cmd_end)
{
	if (cmd_end == 0)
		return (args);
	return (duplicate_args(args, cmd_end));
}
