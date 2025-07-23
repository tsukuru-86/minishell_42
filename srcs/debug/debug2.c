/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:49:23 by muiida            #+#    #+#             */
/*   Updated: 2025/07/23 16:39:12 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "debug/debug.h"

void	debug_print_command_args(char **args)
{
	int	i;

	if (!args)
		return ;
	dbg_printf("Command arguments:\n");
	i = 0;
	while (args[i] != NULL)
	{
		dbg_printf("[%d]=%s ", i, args[i]);
		i++;
	}
	dbg_printf("\n");
}
