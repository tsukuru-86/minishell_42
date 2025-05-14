/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 00:58:32 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/14 04:54:53 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static volatile sig_atomic_t	g_signal = 0;

void	signal_handler(int signum)
{
	g_signal = signum;
	if (signum == SIGINT) // Ctrl+C
	{
		write(1, "\nminishell > ", 12);
		rl_on_new_line();
		rl_redisplay();
	}
}

static void	initialize_environment(char **envp)
{
	*g_env() = create_env_list(envp);
	if (!g_env())
	{
		ft_putstr_fd("minishell: failed to initialize environment\n", 2);
		exit(1);
	}
}

static void	handle_input(char *input, int *status)
{
	t_token		*tokens;
	t_command	*cmd;

	if (*input)
		add_history(input);
	tokens = tokenize(input);
	if (tokens)
	{
		cmd = parse_tokens(tokens);
		if (cmd)
		{
			*status = excute_commands(cmd);
			free_command(cmd);
		}
		free_tokens(tokens);
	}
	free(input);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	int		status;

	(void)argc;
	(void)argv;
	initialize_environment(envp);
	status = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		g_signal = 0;
		input = readline("minishell > ");
		if (!input)
			break ;
		handle_input(input, &status);
	}
	clear_history();
	free_env_list(*g_env());
	return (status);
}
