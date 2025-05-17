/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:36:29 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/18 03:34:20 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* パイプラインコマンドの実行 */
int	execute_pipeline_command(t_command *cmd, t_minishell *shell)
{
	int			status;
	t_command	*current;
	int			pipeline_result;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (execute_single_command(cmd, shell));
	setup_parent_signals();
	pipeline_result = setup_pipeline(cmd);
	if (pipeline_result == 0)
	{
		ft_printf_fd(2, "minishell: pipeline setup failed\n");
		shell->exit_status = 1;
		return (1);
	}
	status = wait_pipeline(cmd);
	current = cmd;
	while (current)
	{
		cleanup_pipeline(current);
		current = current->next;
	}
	init_signal_handlers();
	shell->exit_status = status;
	return (status);
}
