/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 17:45:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/15 17:16:08 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"

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
			debug_print_with_str("Processing line: ", lines[i]);
			handle_input(lines[i], status);
		}
		i++;
	}
}

void	process_with_fallback(char *input, int *status)
{
	char	**lines;

	debug_print("Pipe setup failed, using line processing");
	lines = ft_split(input, '\n');
	if (lines)
	{
		process_input_lines(lines, status);
		free_string_array(lines);
	}
}
