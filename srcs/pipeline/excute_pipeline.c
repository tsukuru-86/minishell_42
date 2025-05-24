/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:58:32 by muiida       +#+  #+#    #+#             */
/*   Updated: 2025/05/25 03:04:39 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipeline.h"

/* パイプラインコマンドを実行する関数 */
int	execute_command_pipeline(t_command *cmd)
{
	int	pipeline_result;
	int	status;

	pipeline_result = setup_pipeline(cmd);
	if (pipeline_result == 0)
	{
		ft_putstr_fd((char *)"minishell: pipeline setup error\n", 2);
		return (1);
	}
	status = wait_pipeline(cmd);
	cleanup_pipeline_commands(cmd);
	return (status);
}
