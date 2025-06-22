/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 20:18:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/22 12:53:23 by muiida           ###   ########.fr       */
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

static char	*process_heredoc_input(char *input)
{
	char	*heredoc_pos;
	char	*line_end;
	char	*delimiter_end;
	char	*command_start;
	char	*result;

	heredoc_pos = ft_strnstr(input, "<<", ft_strlen(input));
	if (!heredoc_pos)
		return (ft_strdup(input));
	line_end = ft_strchr(input, '\n');
	if (!line_end)
		return (ft_strdup(input));
	delimiter_end = heredoc_pos + 2;
	while (*delimiter_end == ' ' || *delimiter_end == '\t')
		delimiter_end++;
	while (*delimiter_end && *delimiter_end != ' ' && *delimiter_end != '\n')
		delimiter_end++;
	command_start = delimiter_end;
	while (*command_start == ' ' || *command_start == '\t')
		command_start++;
	result = ft_substr(command_start, 0, line_end - command_start);
	debug_print_with_str("[DEBUG] Extracted command: ", result,
		DEBUG_ENABLED);
	return (result);
}

static int	handle_non_interactive(int *status)
{
	char	*input;
	char	*processed_input;

	debug_print("[DEBUG] Enter handle_non_interactive (heredoc-aware)\n",
		DEBUG_ENABLED);
	input = read_full_input();
	if (!input || ft_strlen(input) == 0)
	{
		if (input)
			free(input);
		return (0);
	}
	debug_print_with_str("[DEBUG] Non-interactive full input: '%s'\n",
		input, DEBUG_ENABLED);
	processed_input = process_heredoc_input(input);
	if (processed_input)
	{
		handle_input(processed_input, status);
		free(processed_input);
	}
	else
		handle_input(input, status);
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
	if (!isatty(STDIN_FILENO))
		return (get_exit_status());
	return (status);
}
