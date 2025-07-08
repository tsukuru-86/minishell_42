/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:58:32 by muiida       +#+  #+#    #+#             */
/*   Updated: 2025/07/08 22:38:17 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipeline.h"

/* パイプラインのセットアップ */
static int	validate_pipeline(t_command *cmd)
{
	debug_print("[DEBUG] setup_pipeline: Start.");
	if (!cmd)
	{
		debug_print("[DEBUG] setup_pipeline: Command is NULL, returning 0.");
		return (0);
	}
	if (!cmd->next)
	{
		debug_print("[DEBUG] setup_pipeline: Single command, no pipeline needed,
			returning 1.");
		return (1);
	}
	return (2);
}

static int	setup_pipes_and_processes(t_command *cmd)
{
	if (!create_pipes(cmd))
	{
		ft_putstr_fd((char *)"minishell: failed to create pipes\n", 2);
		debug_print("[DEBUG] setup_pipeline: Failed to create pipes,
			returning 0.");
		return (0);
	}
	debug_print("[DEBUG] setup_pipeline: After create_pipes. Before spawn_pipeline_processes.");
	if (!spawn_pipeline_processes(cmd))
	{
		ft_putstr_fd((char *)"minishell: failed to spawn processes\n", 2);
		debug_print("[DEBUG] setup_pipeline: Failed to spawn processes,
			cleaning up and returning 0.");
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
	debug_print("[DEBUG] setup_pipeline: Initializing pipeline.");
	init_pipeline(cmd);
	debug_print("[DEBUG] setup_pipeline: Before create_pipes.");
	if (!setup_pipes_and_processes(cmd))
		return (0);
	debug_print("[DEBUG] setup_pipeline: After spawn_pipeline_processes. Before close_parent_pipes.");
	close_parent_pipes(cmd);
	debug_print("[DEBUG] setup_pipeline: After close_parent_pipes. Returning 1.");
	return (1);
}

/* パイプラインコマンドを実行する関数 */
int	execute_command_pipeline(t_command *cmd)
{
	int	pipeline_result;
	int	status;

	debug_print("[DEBUG] execute_command_pipeline: Start.");
	if (!cmd)
	{
		debug_print("[DEBUG] execute_command_pipeline: Command is NULL,
			returning 1.");
		return (1);
	}
	debug_print("[DEBUG] execute_command_pipeline: Before setup_pipeline.");
	pipeline_result = setup_pipeline(cmd);
	debug_print_with_int("[DEBUG] execute_command_pipeline: After setup_pipeline,
		result: ", pipeline_result);
	if (pipeline_result == 0)
	{
		ft_putstr_fd((char *)"minishell: pipeline setup error\n", 2);
		debug_print("[DEBUG] execute_command_pipeline: Pipeline setup error,
			cleaning up and returning 1.");
		cleanup_pipeline_list_all(cmd);
		return (1);
	}
	debug_print("[DEBUG] execute_command_pipeline: Before wait_pipeline.");
	status = wait_pipeline(cmd);
	debug_print_with_int("[DEBUG] execute_command_pipeline: After wait_pipeline,
		status: ", status);
	if (cmd && cmd->redirects)
	{
		debug_print("[DEBUG] execute_command_pipeline: Cleaning up heredocs.");
		cleanup_heredocs(cmd->redirects);
	}
	debug_print("[DEBUG] execute_command_pipeline: Cleaning up pipeline list all.");
	cleanup_pipeline_list_all(cmd);
	debug_print_with_int("[DEBUG] execute_command_pipeline: Exit with status: ",
		status);
	return (status);
}
