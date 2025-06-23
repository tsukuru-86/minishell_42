/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:36:29 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/23 23:55:11 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipeline.h"

/* シグナルによる終了状態を処理 */
static int	handle_signaled_status(int status)
{
	if (WTERMSIG(status) == SIGPIPE)
		ft_putstr_fd("Broken pipe\n", STDERR_FILENO);
	return (128 + WTERMSIG(status));
}

/* 単一プロセスの待機と状態処理 */
static int	wait_single_process(t_command *current)
{
	int	status;

	if (waitpid(current->pipe.pid, &status, 0) == -1)
	{
		perror("minishell: waitpid");
		return (1);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (handle_signaled_status(status));
	return (0);
}

/* パイプラインの完了を待機 */
int	wait_pipeline(t_command *cmd)
{
	t_command	*current;
	int			last_status;

	if (!cmd)
		return (0);
	current = cmd;
	last_status = 0;
	while (current)
	{
		if (current->pipe.pid != -1)
			last_status = wait_single_process(current);
		current = current->next;
	}
	return (last_status);
}
