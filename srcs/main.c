/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 00:58:32 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/18 23:25:04 by muiida           ###   ########.fr       */
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

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static int	initialize_shell(char **envp)
{
	*g_env() = create_env_list(envp);
	if (!g_env())
	{
		ft_putstr_fd((char *)"minishell: failed to initialize environment\n",
			2);
		return (1);
	}
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}

static void	process_command(char *input, int *status)
{
	t_token		*tokens;
	t_command	*cmd;

	tokens = tokenize(input);
	if (!tokens)
		return ;
	cmd = parse_tokens(tokens);
	if (cmd)
	{
		*status = excute_commands(cmd);
		free_command(cmd);
	}
	free_tokens(tokens);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	int		status;

	(void)argc;
	(void)argv;
	if (initialize_shell(envp))
		return (1);
	status = 0;
	while (1)
	{
		g_signal = 0;
		input = readline("minishell > ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		process_command(input, &status);
		free(input);
	}
	clear_history();
	free_env_list();
	return (status);
}
