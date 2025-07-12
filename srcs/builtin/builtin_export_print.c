/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 17:03:13 by muiida            #+#    #+#             */
/*   Updated: 2025/07/13 06:01:40 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"

/* Print the environment variable list */
void	print_env_list(t_env *head)
{
	while (head != NULL)
	{
		if (ft_strncmp(head->name, "?", 2) != 0)
		{
			ft_printf_fd(STDOUT_FILENO, "declare -x %s", head->name);
			if (head->value)
			{
				ft_printf_fd(STDOUT_FILENO, "=\"%s\"", head->value);
			}
			ft_printf_fd(STDOUT_FILENO, "\n");
		}
		head = head->next;
	}
}
