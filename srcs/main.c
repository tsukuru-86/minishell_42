/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:37:10 by muiida    	+#+    #+#    #+#             */
/*   Updated: 2025/06/11 14:10:12 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_signal = 0;

/* シグナル処理を行うハンドラ関数。
   Ctrl+C（SIGINT）を受け取った場合、新しいプロンプトを表示する */
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

/* シェルの初期化を行う関数。
   環境変数リストの作成とシグナルハンドラの設定を行う */
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

static void	handle_input(char *input, int *status)
{
	t_token	*tokens;
	char	*trimmed;

	if (!input)
		return ;
	trimmed = ft_strtrim(input, " \t\n\r");
	if (!trimmed || !*trimmed)
	{
		if (trimmed)
			free(trimmed);
		return ;
	}
	add_history(input);
	tokens = tokenize(trimmed, NULL);
	if (!tokens)
	{
		free(trimmed);
		*status = 2;
		return ;
	}
	*status = handle_tokens_and_parse(tokens);
	free(trimmed);
}

int	main_loop(void)
{
	char	*input;
	int		status;
	char	*line;

	while (1)
	{
		g_signal = 0;
		if (isatty(fileno(stdin)))
			input = readline("minishell> ");
		else
		{
			line = get_next_line(fileno(stdin));
			if (!line)
				break ;
			input = ft_strtrim(line, "\n");
			free(line);
			if (!input)
				break ;
		}
		if (!input)
			break ;
		handle_input(input, &status);
		free(input);
	}
	return (status);
}
/* メインプログラム。シェルを初期化し、ユーザー入力を
   繰り返し受け取り処理する無限ループを実行する */

int	main(int argc, char **argv, char **envp)
{
	int	status;

	(void)argc;
	(void)argv;
	if (!initialize_shell(envp))
		return (EXIT_FAILURE);
	load_history_file();
	status = main_loop();
	save_history_file();
	clear_history();
	free_env_list();
	return (status);
}
