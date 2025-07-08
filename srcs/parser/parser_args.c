/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 21:40:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/09 02:08:41 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static char	**resize_and_copy_args(char **old_args, int current_arg_count)
{
	char	**new_args;
	int		i;

	new_args = (char **)malloc(sizeof(char *) * (current_arg_count + 2));
	if (!new_args)
		return (NULL);
	i = 0;
	if (old_args)
	{
		while (i < current_arg_count)
		{
			new_args[i] = old_args[i];
			i++;
		}
	}
	return (new_args);
}

int	add_argument(t_command *cmd, char *arg)
{
	int		i;
	char	**new_args;

	if (!cmd || !arg)
		return (1);
	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
			i++;
	}
	new_args = resize_and_copy_args(cmd->args, i);
	if (!new_args)
		return (0);
	new_args[i] = ft_strdup(arg);
	if (!new_args[i])
	{
		free(new_args);
		return (0);
	}
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	return (1);
}
