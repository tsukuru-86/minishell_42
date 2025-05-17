/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 06:50:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/18 03:34:33 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 子プロセス用のシグナルハンドラー設定 */
void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/* 親プロセス用のシグナルハンドラー設定 */
void	setup_parent_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

/* シグナルハンドラー関数 */
static void	handle_interrupt_signal(int signum)
{
	t_minishell	*shell;

	if (signum == SIGINT)
	{
		set_signal_num(signum); // グローバル変数にシグナル番号を設定
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_line_buffer[0] = '\0'; // 入力行を空にする
		rl_point = 0;             // カーソル位置をリセット
		rl_on_new_line();
		rl_redisplay();
		shell = *shell_state_holder();
		if (shell)
			shell->exit_status = 130;
		// 重要: readlineを中断して新しいプロンプトを表示させる
		// rl_done = 1; // これによりreadlineが終了し、新しいプロンプトが表示される
	}
}

/* シグナルハンドラーの初期設定 */
void	init_signal_handlers(void)

{
	signal(SIGINT, handle_interrupt_signal);
	signal(SIGQUIT, SIG_IGN);
}

/* デフォルトのシグナルハンドラーに戻す */
void	restore_default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
