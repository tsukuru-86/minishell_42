/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:37:10 by muiida    	+#+    #+#    #+#             */
/*   Updated: 2025/06/08 15:12:41 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/error_messages.h"
#include "minishell.h"

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
	t_token		*tokens;
	t_command	*cmd;

	if (*input)
		add_history(input);
	cmd = NULL;
	tokens = tokenize(input, cmd);
	(void)tokens;
	if (tokens)
	{
		cmd = parse_tokens(tokens);
		(void)cmd;
		if (cmd)
		{
			*status = excute_commands(cmd);
			set_exit_status(cmd, *status);
			free_command(cmd);
		}
		else
			ft_putstr_fd((char *)ERR_SYNTAX_ERROR, 2);
		free_tokens(tokens);
	}
}

int	main_loop(void)
{
	char	*input;
	int		status;
	char	*line;

	status = 0;
	while (1)
	{
		g_signal = 0;
		if (isatty(fileno(stdin)))
		{
			input = readline("minishell> ");
			if (!input)
			{
				ft_putstr_fd("exit\n", STDERR_FILENO);
				break;
			}
		}
		else
		{
			line = get_next_line(fileno(stdin));
			if (!line)
				break;
			input = ft_strtrim(line, "\n");
			free(line);
		}
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
	status = main_loop();
	clear_history();
	free_env_list();
	return (status);
}
