/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 01:55:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/15 01:55:21 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_accumulated_arg(char **accumulated_arg_ptr)
{
	if (accumulated_arg_ptr && *accumulated_arg_ptr)
	{
		free(*accumulated_arg_ptr);
		*accumulated_arg_ptr = NULL;
	}
}

bool	handle_memory_error(void *ptr, char **accumulated_arg_ptr)
{
	if (!ptr)
	{
		free_accumulated_arg(accumulated_arg_ptr);
		return (false);
	}
	return (true);
}

void	free_command_content(t_command *cmd)
{
	t_redirect	*current;
	t_redirect	*next;

	if (cmd->args)
	{
		free(cmd->args);
		cmd->args = NULL;
	}
	current = cmd->redirects;
	while (current)
	{
		next = current->next;
		if (current->file)
			free(current->file);
		free(current);
		current = next;
	}
	cmd->redirects = NULL;
}

bool	add_accumulated_arg(t_command *cmd_ptr, char **accumulated_arg_ptr,
		const char *error_msg)
{
	if (*accumulated_arg_ptr != NULL)
	{
		if (add_argument(cmd_ptr, *accumulated_arg_ptr) != RET_SUCCESS)
		{
			free(*accumulated_arg_ptr);
			*accumulated_arg_ptr = NULL;
			ft_printf_fd(2, "minishell: %s\n", error_msg);
			return (false);
		}
		*accumulated_arg_ptr = NULL;
	}
	return (true);
}
