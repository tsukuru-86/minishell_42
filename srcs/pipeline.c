/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:36:29 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/14 02:19:54 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	execute_pipeline_command(t_command *cmd, t_command *current);

/* パイプラインの初期化 */
static void	init_pipeline(t_command *cmd)
{
	t_command	*current;

	current = cmd;
	while (current)
	{
		current->pipe.read_fd = -1;
		current->pipe.write_fd = -1;
		current->pipe.pid = -1;
		current = current->next;
	}
}

/* パイプの作成と接続 */
static int	create_pipes(t_command *cmd)
{
	t_command	*current;
	int			pipefd[2];

	current = cmd;
	while (current && current->next)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			cleanup_pipeline(cmd);
			return (0);
		}
		current->pipe.write_fd = pipefd[1];
		current->next->pipe.read_fd = pipefd[0];
		current = current->next;
	}
	return (1);
}

/* 子プロセスの作成と実行 */
static int	spawn_pipeline_processes(t_command *cmd)
{
	t_command	*current;
	pid_t		pid;

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
			execute_pipeline_command(cmd, current);
			exit(EXIT_FAILURE);
		}
		else
			current->pipe.pid = pid;
		current = current->next;
	}
	return (1);
}

/* 親プロセスでのパイプの閉鎖 */
static void	close_parent_pipes(t_command *cmd)
{
	t_command	*current;

	current = cmd;
	while (current)
	{
		if (current->pipe.read_fd != -1)
			close(current->pipe.read_fd);
		if (current->pipe.write_fd != -1)
			close(current->pipe.write_fd);
		current = current->next;
	}
}

/* パイプラインのセットアップ */
int	setup_pipeline(t_command *cmd)
{
	if (!cmd->next)
		return (1);
	init_pipeline(cmd);
	if (!create_pipes(cmd))
		return (0);
	if (!spawn_pipeline_processes(cmd))
		return (0);
	close_parent_pipes(cmd);
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

/* Redirect stdin and stdout based on pipeline pipes */
void	pipeline_redirect_io(t_command *current)
{
	if (current->pipe.read_fd != -1)
	{
		if (dup2(current->pipe.read_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 read");
			exit(EXIT_FAILURE);
		}
	}
	if (current->pipe.write_fd != -1)
	{
		if (dup2(current->pipe.write_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 write");
			exit(EXIT_FAILURE);
		}
	}
}

/* Execute current command within pipeline: handle redirection and run */
void	pipeline_execute_command_logic(t_command *current)
{
	int	status;

	if (current->redirects && !setup_redirection(current->redirects))
		exit(EXIT_FAILURE);
	if (is_builtin(current->args[0]))
	{
		status = execute_builtin(current->args);
		if (current->redirects)
			restore_redirection(current->redirects);
		exit(status);
	}
	else
	{
		status = execute_external_command(current->args);
		exit(status);
	}
}

/* Execute a pipeline command in child: set up env, redirect I/O, close pipes,
	then run */
static void	execute_pipeline_command(t_command *cmd, t_command *current)
{
	setup_pipeline_child_env();
	pipeline_redirect_io(current);
	pipeline_close_pipes(cmd);
	pipeline_execute_command_logic(current);
}
