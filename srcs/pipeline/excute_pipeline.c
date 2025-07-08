/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:58:32 by muiida       +#+  #+#    #+#             */
/*   Updated: 2025/07/09 02:21:14 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline.h"

/* パイプラインのセットアップ */
static int	validate_pipeline(t_command *cmd)
{
	if (!cmd)
	{
		return (0);
	}
	if (!cmd->next)
	{
		// no pipeline needed,
		// returning 1.");
		return (1);
	}
	return (2);
}

static int	setup_pipes_and_processes(t_command *cmd)
{
	if (!create_pipes(cmd))
	{
		ft_putstr_fd((char *)"minishell: failed to create pipes\n", 2);
		// returning 0.");
		return (0);
	}
	if (!spawn_pipeline_processes(cmd))
	{
		ft_putstr_fd((char *)"minishell: failed to spawn processes\n", 2);
		// cleaning up and returning 0.");
		cleanup_pipeline(cmd);
		return (0);
	}
	return (1);
}

int	setup_pipeline(t_command *cmd)
{
	int	validation_result;

	validation_result = validate_pipeline(cmd);
	if (validation_result != 2)
		return (validation_result);
	init_pipeline(cmd);
	if (!setup_pipes_and_processes(cmd))
		return (0);
	close_parent_pipes(cmd);
	return (1);
}

/* パイプラインコマンドを実行する関数 */
int	execute_command_pipeline(t_command *cmd)
{
	int	pipeline_result;
	int	status;

	if (!cmd)
	{
		// returning 1. ");
		return (1);
	}
	pipeline_result = setup_pipeline(cmd);
	// result: ", pipeline_result);
	if (pipeline_result == 0)
	{
		ft_putstr_fd((char *)"minishell: pipeline setup error\n", 2);
		// cleaning up and returning 1. ");
		cleanup_pipeline_list_all(cmd);
		return (1);
	}
	status = wait_pipeline(cmd);
	// status:
	if (cmd && cmd->redirects)
	{
		cleanup_heredocs(cmd->redirects);
	}
	cleanup_pipeline_list_all(cmd);
	// status);
	return (status);
}
