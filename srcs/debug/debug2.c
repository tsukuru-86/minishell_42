/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:49:23 by muiida            #+#    #+#             */
/*   Updated: 2025/07/16 05:38:50 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils/debug.h"

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
