/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:49:23 by muiida            #+#    #+#             */
/*   Updated: 2025/07/14 03:44:44 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	debug_print(const char *message)
{
	if (DEBUG_ENABLED)
		ft_printf_fd(STDERR_FILENO, "[DEBUG]%s\n", message);
}

void	debug_print_with_str(const char *prefix, const char *str)
{
	if (DEBUG_ENABLED)
		ft_printf_fd(STDERR_FILENO, "[DEBUG]%s'%s'\n", prefix, str);
}

void	debug_print_with_int(const char *prefix, int value)
{
	if (DEBUG_ENABLED)
		ft_printf_fd(STDERR_FILENO, "%s%d\n", prefix, value);
}

void	debug_print_command_args(char **args)
{
	int	i;

	if (!DEBUG_ENABLED || !args)
		return ;
	debug_print("[DEBUG] Command arguments:\n");
	i = 0;
	while (args[i])
	{
		debug_print_with_int("i=", i);
		debug_print_with_str("args[i]=", args[i]);
		i++;
	}
}
