/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 05:02:31 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/11 01:31:16 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

/* Check if command is a builtin */
int	is_builtin(char *cmd)
{
	char	**builtins;
	int		i;

	builtins = (char *[]){"echo", "cd", "pwd", "export", "unset", "env", "exit",
		NULL};
	i = 0;
	while (builtins[i])
	{
		if (strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	execute_builtin(char **args)
{
	if (strcmp(args[0], "echo") == 0)
		return (builtin_echo(args));
	else if (strcmp(args[0], "cd") == 0)
		return (builtin_cd(args));
	else if (strcmp(args[0], "pwd") == 0)
		return (builtin_pwd(args));
	else if (strcmp(args[0], "export") == 0)
		return (builtin_export(args));
	else if (strcmp(args[0], "unset") == 0)
		return (builtin_unset(args));
	else if (strcmp(args[0], "env") == 0)
		return (builtin_env(args));
	else if (strcmp(args[0], "exit") == 0)
		return (builtin_exit(args));
	return (1);
}

/* Run command with optional redirection */
static int	run_command(char **cmd, char **envp, t_redirect *redirect)
{
	int	status;

	if (redirect && !setup_redirection(redirect))
	{
		free_redirect(redirect);
		return (1);
	}
	if (is_builtin(cmd[0]))
		status = execute_builtin(cmd);
	else
		status = execute_external_command(cmd, envp);
	if (redirect)
	{
		restore_redirection(redirect);
		free_redirect(redirect);
	}
	return (status);
}

/* Execute commands with optional redirection */
int	excute_commands(char **args, char **envp)
{
	t_redirect	*redirect;
	char		**cmd;
	int			cmd_end;
	int			status;

	if (!args || !args[0])
		return (0);
	redirect = parse_redirect(args, &cmd_end);
	cmd = prepare_command(args, cmd_end);
	if (!cmd)
		return (1);
	status = run_command(cmd, envp, redirect);
	if (cmd != args)
		free_tokens(cmd);
	return (status);
}
