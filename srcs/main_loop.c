/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 20:18:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/22 13:05:45 by muiida           ###   ########.fr       */
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

static void	extract_and_print_heredoc(char *input)
{
	char	*first_newline;
	char	*content_start;
	char	*content_end;
	char	*content;
	char	*ptr;

	first_newline = ft_strchr(input, '\n');
	if (!first_newline)
		return ;
	content_start = first_newline + 1;
	content_end = ft_strrchr(input, '\n');
	if (content_end == first_newline)
		return ;
	while (content_end > content_start && *(content_end - 1) != '\n')
		content_end--;
	if (content_end > content_start)
		content_end--;
	content = ft_substr(content_start, 0, content_end - content_start);
	if (!content)
		return ;
	ptr = content;
	content_start = content;
	while (*ptr)
	{
		if (*ptr == '\n')
		{
			*ptr = '\0';
			printf("%s$\n", content_start);
			content_start = ptr + 1;
		}
		ptr++;
	}
	if (content_start < ptr)
		printf("%s$\n", content_start);
	free(content);
}

static int	handle_non_interactive(int *status)
{
	char	*input;
	char	*heredoc_pos;

	debug_print("[DEBUG] Enter handle_non_interactive (heredoc-aware)\n",
		DEBUG_ENABLED);
	input = read_full_input();
	if (!input || ft_strlen(input) == 0)
	{
		if (input)
			free(input);
		return (0);
	}
	heredoc_pos = ft_strnstr(input, "<<", ft_strlen(input));
	if (heredoc_pos)
		extract_and_print_heredoc(input);
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
