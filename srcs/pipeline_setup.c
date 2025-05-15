/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_setup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:50:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/15 08:39:40 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
			execute_pipeline_command(current);
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
