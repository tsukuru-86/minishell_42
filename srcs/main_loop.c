/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 20:18:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/23 16:39:12 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils/utils.h"
#include "debug/debug.h"

void	handle_input(char *input, int *status)
{
	int	empty_status;

	dbg_printf("Enter handle_input: %s", input);
	if (!input)
		return ;
	if (!check_input_line_limit(input))
	{
		*status = 1;
		return ;
	}
	empty_status = handle_empty_input(input);
	if (empty_status != -1)
	{
		*status = empty_status;
		if (input && *input)
			add_history(input);
		return ;
	}
	dbg_printf("Before process_valid_input\n");
	process_valid_input(input, status);
	dbg_printf("After process_valid_input\n");
	dbg_printf("Exit handle_input\n");
}

static int	handle_interactive(int *status)
{
	char	*input;

	input = readline("minishell> ");
	if (!input)
	{
		if (isatty(STDIN_FILENO))
			write(2, "exit\n", 5);
		return (0);
	}
	dbg_printf("Interactive input: %s", input);
	handle_input(input, status);
	dbg_printf("Status: %d", *status);
	free(input);
	return (1);
}

static int	handle_non_interactive(int *status)
{
	char	*line;

	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (0);
	dbg_printf("NI input: %s", line);
	handle_input(line, status);
	dbg_printf("Status: %d", *status);
	free(line);
	return (1);
}

int	main_loop(void)
{
	int	status;

	status = 0;
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
		}
	}
	if (!isatty(STDIN_FILENO))
		return (get_exit_status());
	return (status);
}
