/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_cleanup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:51:42 by muiida            #+#    #+#             */
/*   Updated: 2025/07/09 02:21:14 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline.h"

/* パイプラインのクリーンアップ */
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

/* パイプラインのクリーンアップを行う関数 */
void	cleanup_pipeline_list_all(t_command *cmd)
{
	t_command	*current;

	current = cmd;
	while (current)
	{
		cleanup_pipeline(current);
		current = current->next;
	}
}
