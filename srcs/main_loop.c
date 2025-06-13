/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 20:18:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/13 20:22:55 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils/input_utils.h"

static int	handle_interactive(int *status)
{
	char	*input;

	input = readline("minishell> ");
	if (!input)
		return (0);
	handle_input(input, status);
	free(input);
	return (1);
}

static int	handle_non_interactive(int *status)
{
	char	*line;
	char	*input;

	line = get_next_line(0);
	if (!line)
		return (0);
	input = ft_strtrim(line, "\n");
	free(line);
	if (!input)
		return (0);
	handle_input(input, status);
	free(input);
	return (1);
}

int	main_loop(void)
{
	int	status;

	while (1)
	{
		g_signal = 0;
		if (isatty(STDIN_FILENO))
		{
			if (!handle_interactive(&status))
				break ;
		}
		else
		{
			if (!handle_non_interactive(&status))
				break ;
			break ;
		}
	}
	return (status);
}
