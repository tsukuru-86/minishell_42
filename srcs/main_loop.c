/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 20:18:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/01 23:52:38 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils/input_utils.h"

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
	debug_print_with_str("[DEBUG] Interactive input: ", input);
	debug_print("[DEBUG] Calling handle_input (interactive)...");
	handle_input(input, status);
	debug_print_with_int("[DEBUG] Status after handle_input (interactive): ", *status);
	free(input);
	return (1);
}

static int	handle_non_interactive(int *status)
{
	char	*line;

	line = get_next_line(0);
	if (!line)
		return (0);
	debug_print_with_str("[DEBUG] NI input: ", line);
	debug_print("[DEBUG] Calling handle_input (non-interactive)...");
	handle_input(line, status);
	debug_print_with_int("[DEBUG] Status after handle_input (non-interactive): ", *status);
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
			debug_print("[DEBUG] Entering interactive mode loop.");
			if (!handle_interactive(&status))
				break ;
		}
		else
		{
			debug_print("[DEBUG] Entering non-interactive mode loop.");
			if (!handle_non_interactive(&status))
				break ;
		}
	}
	if (!isatty(STDIN_FILENO))
		return (get_exit_status());
	return (status);
}
