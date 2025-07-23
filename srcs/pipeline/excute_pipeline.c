/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:58:32 by muiida       +#+  #+#    #+#             */
/*   Updated: 2025/07/23 19:36:50 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipeline.h"

/* Set up the pipeline */
int	setup_pipeline(t_command *cmd)
{
	if (!cmd)
		return (0);
	if (!cmd->next)
		return (1);
	init_pipeline(cmd);
	if (!create_pipes(cmd))
	{
		ft_putstr_fd((char *)"minishell: failed to create pipes\n", 2);
		return (0);
	}
	if (!spawn_pipeline_processes(cmd))
	{
		ft_putstr_fd((char *)"minishell: failed to spawn processes\n", 2);
		cleanup_pipeline(cmd);
		return (0);
	}
	close_parent_pipes(cmd);
	return (1);
}

/* Execute pipeline commands */
int	execute_command_pipeline(t_command *cmd)
{
	int			pipeline_result;
	int			status;
	t_command	*cur;

	if (!cmd)
		return (1);
	pipeline_result = setup_pipeline(cmd);
	if (pipeline_result == 0)
	{
		cur = cmd;
		while (cur)
		{
			if (cur->redirects)
				print_redirect_errors(cur->redirects);
			cur = cur->next;
		}
		ft_putstr_fd((char *)"minishell: pipeline setup error\n", 2);
		cleanup_pipeline_list_all(cmd);
		return (1);
	}
	status = wait_pipeline(cmd);
	cur = cmd;
	while (cur)
	{
		if (cur->redirects)
			print_redirect_errors(cur->redirects);
		cur = cur->next;
	}
	if (cmd && cmd->redirects)
		cleanup_heredocs(cmd->redirects);
	cleanup_pipeline_list_all(cmd);
	return (status);
}
