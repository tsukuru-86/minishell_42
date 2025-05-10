/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute_command_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 01:30:00 by muiida           #+#    #+#             */
/*   Updated: 2025/05/11 01:30:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

/* find redirect position and type in args */
static int	find_redirect(char **args, int *cmd_end)
{
	int	i;

	i = 0;
	*cmd_end = 0;
	while (args[i])
	{
		if (args[i][0] == '>')
		{
			*cmd_end = i;
			if (args[i][1] == '>')
				return (REDIR_APPEND);
			return (REDIR_OUT);
		}
		if (args[i][0] == '<')
		{
			*cmd_end = i;
			return (REDIR_IN);
		}
		i++;
	}
	return (0);
}

/* parse redirect tokens and return redirect struct */
t_redirect	*parse_redirect(char **args, int *cmd_end)
{
	int	type;

	type = find_redirect(args, cmd_end);
	if (type)
		return (create_redirect(type, args[*cmd_end + 1]));
	return (NULL);
}

/* duplicate argument vector up to count */
static char	**duplicate_args(char **args, int count)
{
	char	**cmd;
	int		i;

	cmd = malloc(sizeof(char *) * (count + 1));
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
