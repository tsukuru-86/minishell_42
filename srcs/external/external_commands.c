/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/20 21:40:29 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "external.h"
#include "libft.h"
#include "minishell.h"
#include <unistd.h>

void	launch_child(char *cmd_path, char **args)
{
	char	**env_array;

	debug_print("[DEBUG] launch_child: start", DEBUG_ENABLED);
	debug_print_with_str("[DEBUG] cmd_path: ", cmd_path, DEBUG_ENABLED);
	debug_print_with_int("[DEBUG] cmd_path ptr: ", (int)(long)cmd_path,
		DEBUG_ENABLED);
	debug_print_command_args(args, DEBUG_ENABLED);
	env_array = env_list_to_array();
	if (!env_array)
	{
		debug_print("[DEBUG] env_list_to_array failed", DEBUG_ENABLED);
		perror("minishell: env_list_to_array failed");
		exit(127);
	}
	debug_print("[DEBUG] before execve", DEBUG_ENABLED);
	execve(cmd_path, args, env_array);
	debug_print("[DEBUG] execve failed", DEBUG_ENABLED);
	ft_printf_fd(STDERR_FILENO, "minishell: execve failed: %s: %s\n", cmd_path,
		strerror(errno));
	if (errno == EACCES)
		exit(126);
	exit(127);
}
