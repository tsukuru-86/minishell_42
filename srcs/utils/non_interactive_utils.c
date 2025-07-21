/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 17:45:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/16 04:33:24 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"
#include "debug.h"

void	free_string_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	process_input_lines(char **lines, int *status)
{
	int	i;

	i = 0;
	while (lines[i])
	{
		if (ft_strlen(lines[i]) > 0)
		{
			dbg_printf("Processing line: %s", lines[i]);
			handle_input(lines[i], status);
		}
		i++;
	}
}

void	process_with_fallback(char *input, int *status)
{
	char	**lines;

	dbg_printf("Pipe setup failed, using line processing");
	lines = ft_split(input, '\n');
	if (lines)
	{
		process_input_lines(lines, status);
		free_string_array(lines);
	}
}
