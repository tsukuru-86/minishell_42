/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 20:18:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/17 08:34:55 by muiida           ###   ########.fr       */
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
	debug_print_with_str("[DEBUG] Interactive input: ", input, DEBUG_ENABLED);
	handle_input(input, status);
	debug_print_with_int("[DEBUG] Status: ", *status, DEBUG_ENABLED);
	free(input);
	return (1);
}

static char	*read_full_input(void)
{
	char	*line;
	char	*full_input;
	char	*temp;

	full_input = ft_strdup("");
	if (!full_input)
		return (NULL);
	line = get_next_line(0);
	while (line != NULL)
	{
		temp = full_input;
		full_input = ft_strjoin(full_input, line);
		free(temp);
		free(line);
		if (!full_input)
			return (NULL);
		line = get_next_line(0);
	}
	return (full_input);
}

static int	handle_non_interactive(int *status)
{
	char	*input;

	input = read_full_input();
	if (!input)
		return (0);
	if (ft_strlen(input) == 0)
	{
		free(input);
		return (0);
	}
	debug_print_with_str("[DEBUG] Non-inter input: ", input, DEBUG_ENABLED);
	handle_input(input, status);
	debug_print_with_int("[DEBUG] Status: ", *status, DEBUG_ENABLED);
	free(input);
	return (0);
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
			handle_non_interactive(&status);
			break ;
		}
	}
	return (status);
}
