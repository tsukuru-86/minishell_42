/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 00:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/03 03:43:38 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	*get_redirect_name(int type)
{
	if (type == 0)
		return ("OUT");
	if (type == 1)
		return ("IN");
	if (type == 2)
		return ("APPEND");
	if (type == 3)
		return ("HEREDOC");
	return ("UNKNOWN");
}

static void	print_redirect_info(t_redirect *redirect, int depth)
{
	t_redirect	*current;
	int			i;

	current = redirect;
	while (current)
	{
		i = 0;
		while (i < depth)
		{
			printf("  ");
			i++;
		}
		printf("Redirect: type=%s, file='%s'\n",
			get_redirect_name(current->type), current->file);
		current = current->next;
	}
}

static void	print_command_args(char **args)
{
	int	i;

	if (args)
	{
		printf("  Args: [");
		i = 0;
		while (args[i])
		{
			printf("'%s'", args[i]);
			if (args[i + 1])
				printf(", ");
			i++;
		}
		printf("]\n");
	}
	else
		printf("  Args: NULL\n");
}

void	print_command_debug(t_command *cmd)
{
	if (!cmd)
		return ;
	printf("Command:\n");
	print_command_args(cmd->args);
	if (cmd->redirects)
	{
		printf("  Redirections:\n");
		print_redirect_info(cmd->redirects, 2);
	}
	else
		printf("  Redirections: None\n");
	printf("  Last status: %d\n", cmd->last_status);
}

void	print_commands_debug(t_command *commands)
{
	t_command	*current;
	int			cmd_count;

	printf("\n=== PARSER OUTPUT (COMMANDS) ===\n");
	if (!commands)
	{
		printf("No commands parsed\n");
		printf("================================\n\n");
		return ;
	}
	current = commands;
	cmd_count = 0;
	while (current)
	{
		printf("Command[%d]:\n", cmd_count++);
		print_command_debug(current);
		if (current->next)
			printf("  | (pipe to next command)\n");
		printf("\n");
		current = current->next;
	}
	printf("================================\n\n");
}
