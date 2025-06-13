/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:37:10 by muiida            #+#    #+#             */
/*   Updated: 2025/06/13 16:22:05 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils/debug_utils.h"
#include "utils/input_utils.h"

volatile sig_atomic_t	g_signal = 0;

void	signal_handler(int signum)
{
	g_signal = signum;
	if (signum == SIGINT)
	{
		write(1, "\nminishell > ", 12);
		rl_on_new_line();
		rl_redisplay();
	}
}

static int	initialize_shell(char **envp)
{
	*get_env_val() = create_env_list(envp);
	if (!get_env_val())
	{
		ft_putstr_fd((char *)"minishell: failed to initialize environment\n",
			STDERR_FILENO);
		return (0);
	}
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	return (1);
}

static void	handle_input(char *input, int *status, int debug)
{
	t_token	*tokens;
	char	*prepared;

	if (!input)
		return ;
	prepared = prepare_input(input);
	if (!prepared)
		return ;
	add_history(input);
	tokens = tokenize(prepared, NULL);
	free(prepared);
	print_tokens_debug(tokens, debug);
	if (!tokens)
	{
		*status = 2;
		return ;
	}
	*status = handle_tokens_and_parse(tokens);
}

int	main_loop(int debug)
{
	char	*input;
	int		status;
	char	*line;

	while (1)
	{
		g_signal = 0;
		if (isatty(STDIN_FILENO))
			input = readline("minishell> ");
		else
		{
			line = get_next_line(0);
			if (!line)
				break ;
			input = ft_strtrim(line, "\n");
			free(line);
			if (!input)
				break ;
			handle_input(input, &status, debug);
			free(input);
			break ;
		}
		if (!input)
			break ;
		handle_input(input, &status, debug);
		free(input);
	}
	return (status);
}

int	main(int argc, char **argv, char **envp)
{
	int	status;
	int	debug;
	int	i;

	debug = 0;
	i = 1;
	while (i < argc)
	{
		if (ft_strcmp(argv[i], "-d") == 0)
			debug = 1;
		i++;
	}
	if (!initialize_shell(envp))
		return (EXIT_FAILURE);
	load_history_file();
	status = main_loop(debug);
	save_history_file();
	clear_history();
	free_env_list();
	return (status);
}
