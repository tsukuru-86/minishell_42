/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/07/23 16:39:12 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "external.h"
#include "libft.h"
#include "minishell.h"
#include "debug/debug.h"

void	launch_child(char *cmd_path, char **args)
{
	char	**env_array;

	dbg_printf("launch_child: start");
	dbg_printf("cmd_path: %s", cmd_path);
	dbg_printf("cmd_path ptr: %d", (int)(long)cmd_path);
	debug_print_command_args(args);
	env_array = env_list_to_array();
	if (!env_array)
	{
		perror("minishell: env_list_to_array failed");
		exit(127);
	}
	dbg_printf("before execve");
	execve(cmd_path, args, env_array);
	ft_printf_fd(STDERR_FILENO, "minishell: execve failed: %s: %s\n", cmd_path,
		strerror(errno));
	if (errno == EACCES)
		exit(126);
	exit(127);
}
