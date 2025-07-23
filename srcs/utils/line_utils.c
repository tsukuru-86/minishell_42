/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 20:34:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/23 16:39:12 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils/utils.h"
#include "debug/debug.h"

void	free_lines(char **lines)
{
	int	i;

	if (!lines)
		return ;
	i = 0;
	while (lines[i])
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}

void	process_lines(char **lines, int *status)
{
	int	i;

	i = 0;
	while (lines[i])
	{
		dbg_printf("Processing line: %s", lines[i]);
		handle_input(lines[i], status);
		i++;
	}
}
