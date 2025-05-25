/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:37:10 by muiida    	+#+    #+#    #+#             */
/*   Updated: 2025/05/26 00:08:32 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile sig_atomic_t	g_signal = 0;

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

/* 子プロセスのシグナル設定を行う関数。
   子プロセスではSIGINTとSIGQUITをデフォルトの処理に設定する */
void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/* シェルの初期化を行う関数。
   環境変数リストの作成とシグナルハンドラの設定を行う */
static int	initialize_shell(char **envp)
{
	*get_env_val() = create_env_list(envp);
	if (!get_env_val())
	{
		ft_putstr_fd((char *)"minishell: failed to initialize environment\n",
			2);
		return (1);
	}
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}

static void	handle_input(char *input, int *status)
{
	t_token		*tokens;
	t_command	*cmd;

	if (*input)
		add_history(input);
	cmd = NULL;
	tokens = tokenize(input, cmd);
	if (tokens)
	{
		cmd = parse_tokens(tokens);
		if (cmd)
		{
			*status = excute_commands(cmd);
			set_exit_status(cmd, *status);
			free_command(cmd);
		}
		else
			ft_putstr_fd((char *)"minishell: syntax error\n", 2);
		free_tokens(tokens);
	}
}

/* メインプログラム。シェルを初期化し、ユーザー入力を
   繰り返し受け取り処理する無限ループを実行する */
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
		handle_input(input, &status);
		free(input);
	}
	clear_history();
	free_env_list();
	return (status);
}
