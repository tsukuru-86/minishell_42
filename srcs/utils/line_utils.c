/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 20:34:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/19 20:35:27 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils/input_utils.h"

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
		debug_print_with_str("[DEBUG] Processing line: ", lines[i],
			DEBUG_ENABLED);
		handle_input(lines[i], status);
		i++;
	}
}
