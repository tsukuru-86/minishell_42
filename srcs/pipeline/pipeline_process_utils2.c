/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_process_utils2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:04:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/20 09:04:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils/input_utils.h"
#include "pipeline/pipeline.h"

void	handle_empty_args(t_command *current)
{
	if (current->redirects && setup_redirection(current->redirects))
		exit(0);
	exit(127);
}

void	execute_builtin_command(t_command *current)
{
	int	status;

	status = execute_builtin(current->args);
	if (current->redirects)
		restore_redirection(current->redirects);
	exit(status);
}

/*
** fork失敗時の処理
*/
bool	handle_fork_error(t_command *cmd)
{
	perror("fork");
	cleanup_pipeline(cmd);
	return (false);
}
