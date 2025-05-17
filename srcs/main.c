/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:37:10 by muiida    	+#+    #+#    #+#             */
/*   Updated: 2025/05/17 20:57:53 by muiida           ###   ########.fr       */
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

// 古い関数を削除（tokenizer.cに移動）

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	int			status;
	t_token		*tokens;
	t_command	*cmd;

	(void)argc;
	(void)argv;
	// 環境変数の初期化
	*g_env() = create_env_list(envp);
	if (!g_env())
	{
		ft_putstr_fd("minishell: failed to initialize environment\n", 2);
		return (1);
	}
	status = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		g_signal = 0;
		input = readline("minishell > ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		tokens = tokenize(input);
		if (tokens)
		{
			// デバッグ用：トークンの内容を表示
			// print_tokens(tokens);
			// トークンをコマンド構造体に変換
			cmd = parse_tokens(tokens);
			if (cmd)
			{
				status = excute_commands(cmd);
				free_command(cmd);
			}
			free_tokens(tokens);
		}
		free(input);
	}
	clear_history(); // rl_clear_history();
	free_env_list(*g_env());
	return (status);
}
