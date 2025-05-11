/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:36:29 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/12 03:47:45 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 各コマンドの子プロセスでの実行処理 */
static void	execute_pipeline_command(t_command *cmd, t_command *current,
		char **envp)
{
	t_command	*tmp;
	int			status;

	// 子プロセスのシグナルハンドリングを設定
	setup_child_signals();
	// パイプライン実行中であることを示す環境変数を設定
	putenv("MINISHELL_PIPELINE=1");
	// 入力のリダイレクト - 前のコマンドからの入力をSTDINにリダイレクト
	if (current->pipe.read_fd != -1)
	{
		if (dup2(current->pipe.read_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 read");
			exit(EXIT_FAILURE);
		}
	}
	// 出力のリダイレクト - このコマンドの出力を次のコマンドへリダイレクト
	if (current->pipe.write_fd != -1)
	{
		if (dup2(current->pipe.write_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 write");
			exit(EXIT_FAILURE);
		}
	}
	// すべてのパイプを閉じる（必須！）
	tmp = cmd;
	while (tmp)
	{
		if (tmp->pipe.read_fd != -1)
			close(tmp->pipe.read_fd);
		if (tmp->pipe.write_fd != -1)
			close(tmp->pipe.write_fd);
		tmp = tmp->next;
	}
	// リダイレクトの設定（パイプラインより優先）
	if (current->redirects && !setup_redirection(current->redirects))
		exit(EXIT_FAILURE);
	// コマンドの実行
	if (is_builtin(current->args[0]))
	{
		status = execute_builtin(current->args);
		if (current->redirects)
			restore_redirection(current->redirects);
		exit(status);
	}
	else
	{
		status = execute_external_command(current->args, envp);
		exit(status);
	}
}

/* パイプラインのセットアップ */
int	setup_pipeline(t_command *cmd)
{
	int			pipefd[2];
	pid_t		pid;
	t_command	*current;
	extern char	**environ;

	// パイプがない場合は通常の実行
	if (!cmd->next)
		return (1);
	// 初期化
	current = cmd;
	while (current)
	{
		current->pipe.read_fd = -1;
		current->pipe.write_fd = -1;
		current->pipe.pid = -1;
		current = current->next;
	}
	// パイプを作成し、各コマンドに接続
	current = cmd;
	while (current && current->next)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			cleanup_pipeline(cmd);
			return (0);
		}
		// 現在のコマンドの出力はパイプの書き込み側
		current->pipe.write_fd = pipefd[1];
		// 次のコマンドの入力はパイプの読み込み側
		current->next->pipe.read_fd = pipefd[0];
		current = current->next;
	}
	// 各コマンドを子プロセスとして実行
	current = cmd;
	while (current)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			cleanup_pipeline(cmd);
			return (0);
		}
		if (pid == 0)
		{
			// 子プロセスでコマンドを実行
			execute_pipeline_command(cmd, current, environ);
			// ここには到達しない
			exit(EXIT_FAILURE);
		}
		else
		{
			// 親プロセス - 子プロセスIDを保存
			current->pipe.pid = pid;
		}
		current = current->next;
	}
	// 親プロセスですべてのパイプを閉じる
	// これが重要！閉じないとパイプが詰まって次のプロセスが実行されない
	current = cmd;
	while (current)
	{
		if (current->pipe.read_fd != -1)
			close(current->pipe.read_fd);
		if (current->pipe.write_fd != -1)
			close(current->pipe.write_fd);
		current = current->next;
	}
	return (1);
}

/* パイプラインのクリーンアップ */
void	cleanup_pipeline(t_command *cmd)
{
	t_command	*current;

	if (!cmd)
		return ;
	// すべてのコマンドの残っているファイルディスクリプタを閉じる
	current = cmd;
	while (current)
	{
		if (current->pipe.read_fd != -1)
		{
			close(current->pipe.read_fd);
			current->pipe.read_fd = -1;
		}
		if (current->pipe.write_fd != -1)
		{
			close(current->pipe.write_fd);
			current->pipe.write_fd = -1;
		}
		current = current->next;
	}
}

/* パイプラインの完了を待機 */
int	wait_pipeline(t_command *cmd)
{
	t_command	*current;
	int			status;
	int			last_status;

	current = cmd;
	last_status = 0;
	while (current)
	{
		if (current->pipe.pid != -1)
		{
			if (waitpid(current->pipe.pid, &status, 0) == -1)
				perror("waitpid");
			else if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
		}
		current = current->next;
	}
	return (last_status);
}
