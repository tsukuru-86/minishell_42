/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 00:58:32 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/22 01:56:09 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static volatile sig_atomic_t	g_signal = 0;

/* シグナル処理を行うハンドラ関数。
   Ctrl+C（SIGINT）を受け取った場合、新しいプロンプトを表示する */
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

/* コマンド文字列を処理する関数。
   文字列をトークン化し、パースした後、コマンドを実行する */
static void	process_command(char *input, int *status)
{
	t_token		*tokens;
	t_command	*cmd;

	tokens = tokenize(input);
	if (!tokens)
	{
		ft_putstr_fd((char *)"minishell: failed to tokenize input\n", 2);
		return ;
	}
	cmd = parse_tokens(tokens);
	if (cmd)
	{
		*status = excute_commands(cmd);
		free_command(cmd);
	}
	else
		ft_putstr_fd((char *)"minishell: syntax error\n", 2);
	free_tokens(tokens);
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
		if (*input)
		{
			add_history(input);
			process_command(input, &status);
		}
		free(input);
	}
	clear_history();
	free_env_list();
	return (status);
}
