/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_setup_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/05/31 18:56:52 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipeline.h"

/* パイプライン内のすべてのコマンドのパイプ情報を初期化する関数 */
void	init_pipeline(t_command *cmd)
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

/* コマンド間のパイプを作成し、読み書きのファイルディスクリプタを設定する関数 */
int	create_pipes(t_command *cmd)
{
	t_command	*current;
	int			pipefd[2];

	current = cmd;
	while (current && current->next)
	{
		if (pipe(pipefd) == -1)
		{
			perror("minishell: pipe");
			cleanup_pipeline(cmd);
			return (0);
		}
		current->pipe.write_fd = pipefd[1];
		current->next->pipe.read_fd = pipefd[0];
		current = current->next;
	}
	return (1);
}

/* 親プロセスでのパイプの閉鎖 */
void	close_parent_pipes(t_command *cmd)
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
