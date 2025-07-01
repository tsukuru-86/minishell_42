/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/07/01 23:15:52 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"

int	builtin_cd(char **args)
{
	char	*path;
	int		chdir_result;

	debug_print("[DEBUG] builtin_cd: Start.");
	debug_print_command_args(args);
	if (args[1] == NULL)
	{
		path = getenv("HOME");
		debug_print_with_str("[DEBUG] builtin_cd: No argument, using HOME: ", path);
	}
	else
	{
		path = args[1];
		debug_print_with_str("[DEBUG] builtin_cd: Using path: ", path);
	}
	debug_print_with_str("[DEBUG] builtin_cd: Calling chdir with path: ", path);
	chdir_result = chdir(path);
	debug_print_with_int("[DEBUG] builtin_cd: chdir returned: ", chdir_result);
	if (chdir_result == -1)
	{
		ft_printf_fd(STDERR_FILENO, "cd: %s: %s\n", path, strerror(errno));
		debug_print("[DEBUG] builtin_cd: chdir failed, returning 1.");
		return (1);
	}
	debug_print("[DEBUG] builtin_cd: Exit, returning 0.");
	return (0);
}

