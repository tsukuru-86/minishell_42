/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/07/09 02:42:57 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "external.h"
#include "libft.h"
#include "minishell.h"

void	launch_child(char *cmd_path, char **args)
{
	char	**env_array;

	env_array = env_list_to_array();
	if (!env_array)
	{
		perror("minishell: env_list_to_array failed");
		exit(127);
	}
	execve(cmd_path, args, env_array);
	ft_printf_fd(STDERR_FILENO, "minishell: execve failed: %s: %s\n", cmd_path,
		strerror(errno));
	if (errno == EACCES)
		exit(126);
	exit(127);
}
