/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 20:18:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/15 23:27:43 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils/utils.h"

void	handle_input(char *input, int *status)
{
	int	empty_status;

	debug_print_with_str("Enter handle_input: \n", input);
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
	debug_print("Before process_valid_input\n");
	process_valid_input(input, status);
	debug_print("After process_valid_input\n");
	debug_print("Exit handle_input\n");
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
	debug_print_with_str("Interactive input: ", input);
	handle_input(input, status);
	debug_print_with_int("Status: ", *status);
	free(input);
	return (1);
}

static int	handle_non_interactive(int *status)
{
	char	*line;

	line = get_next_line(0);
	if (!line)
		return (0);
	debug_print_with_str("NI input: ", line);
	handle_input(line, status);
	debug_print_with_int("Status: ", *status);
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
