/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/07/14 03:50:17 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "external.h"
#include "libft.h"
#include "minishell.h"

void	launch_child(char *cmd_path, char **args)
{
	char	**env_array;

	debug_print("launch_child: start");
	debug_print_with_str("cmd_path: ", cmd_path);
	debug_print_with_int("cmd_path ptr: ", (int)(long)cmd_path);
	debug_print_command_args(args);
	env_array = env_list_to_array();
	if (!env_array)
	{
		perror("minishell: env_list_to_array failed");
		exit(127);
	}
	debug_print("before execve");
	execve(cmd_path, args, env_array);
	ft_printf_fd(STDERR_FILENO, "minishell: execve failed: %s: %s\n", cmd_path,
		strerror(errno));
	if (errno == EACCES)
		exit(126);
	exit(127);
}
