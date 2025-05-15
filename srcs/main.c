/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 00:58:32 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/15 08:02:31 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <readline/readline.h>
#include <signal.h>

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

// static void	execute_command_light(t_command *cmd)
// {
// 	if (cmd == NULL || cmd->args[0] == NULL)
// 	{
// 		return ;
// 	}
// 	ft_printf_fd(2, "DEBUG: EXECUTING COMMAND: %s\n", cmd->args[0]);
// 	if (is_builtin(cmd->args[0]))
// 	{
// 		execute_builtin(cmd);
// 	}
// 	else if (find_command(cmd->args[0]) != NULL)
// 	{
// 		execute_external_command(cmd);
// 	}
// 	else
// 	{
// 		ft_printf_fd(2, "minishell: %s: command not found\n",
// cmd->args[0]);
// 	}
// }

void	handle_input(void)
{
	char		*input;
	t_token		*tokens;
	t_command	*cmd;

	input = readline("minishell > ");
	// EOF(Ctrl+D) が入力された場合
	if (input == NULL)
		return ;
	ft_printf_fd(2, "DEBUG INPUT: [%s]\n", input);
	tokens = tokenize(input);
	cmd = parse_tokens(tokens);
	if (cmd != NULL)
	{
		execute_command(cmd);
		// execute_command_light(cmd);
		free_command(cmd); // コマンドを解放
	}
	free(input); // 入力文字列を解放
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	int		status;

	(void)argc;
	(void)argv;
	initialize_environment(envp);
	status = 0;
	setup_signal_handlers();
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		g_signal = 0;
		input = readline("minishell > ");
		if (!input)
			break ;
		// デバッグ用：入力内容を即座に表示
		ft_printf_fd(2, "DEBUG INPUT: [%s]\n", input);
		handle_input();
	}
	clear_history();
	free_env_list(*g_env());
	return (status);
}
