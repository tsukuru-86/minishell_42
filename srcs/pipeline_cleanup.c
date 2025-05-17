/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_cleanup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:50:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/16 04:28:16 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* パイプラインのクリーンアップを行う関数 */
void	cleanup_pipeline(t_command *cmd)
{
	t_command	*current;

	if (!cmd)
		return ;
	current = cmd;
	while (current)
	{
		if (current->pipe.read_fd != -1)
		{
			if (close(current->pipe.read_fd) == -1)
				perror("close");
			current->pipe.read_fd = -1;
		}
		if (current->pipe.write_fd != -1)
		{
			if (close(current->pipe.write_fd) == -1)
				perror("close");
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
			{
				perror("waitpid");
				return (-1);
			}
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		current = current->next;
	}
	return (last_status);
}

/* パイプのクローズ処理 */
void	pipeline_close_pipes(t_command *cmd)
{
	t_command	*tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->pipe.read_fd != -1)
		{
			if (close(tmp->pipe.read_fd) == -1)
			{
				perror("close");
				exit(EXIT_FAILURE);
			}
			tmp->pipe.read_fd = -1;
		}
		if (tmp->pipe.write_fd != -1)
		{
			if (close(tmp->pipe.write_fd) == -1)
			{
				perror("close");
				exit(EXIT_FAILURE);
			}
			tmp->pipe.write_fd = -1;
		}
		tmp = tmp->next;
	}
}
