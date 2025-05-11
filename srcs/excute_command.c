/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 05:02:31 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/11 22:19:49 by muiida           ###   ########.fr       */
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

/* トークンリストを文字列配列に変換 */
static char	**tokens_to_array(t_token *tokens)
{
	t_token	*current;
	int		count;
	char	**array;
	int		i;

	count = 0;
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
			count++;
		current = current->next;
	}
	array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	i = 0;
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			array[i] = ft_strdup(current->content);
			if (!array[i])
			{
				while (--i >= 0)
					free(array[i]);
				free(array);
				return (NULL);
			}
			i++;
		}
		current = current->next;
	}
	array[i] = NULL;
	return (array);
}

/* 文字列配列の解放 */
static void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static t_redirect	*parse_redirect(char **args, int *cmd_end)
{
	char	**builtins;
	int		i;

	builtins = (char *[]){"echo", "cd", "pwd", "export", "unset", "env", "exit",
		NULL};
	i = 0;
	while (builtins[i])
	{
		if (strcmp(cmd, builtins[i]) == 0)
			return (NULL);
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

/* コマンドの実行 */
int	excute_commands(t_token *tokens, char **envp)
{
	t_redirect	*redirect;
	char		**args;
	char		**cmd;
	int			cmd_end;
	int			status;

	if (!tokens)
		return (0);
	// トークンリストを配列に変換
	args = tokens_to_array(tokens);
	if (!args)
		return (1);
	// リダイレクションの解析
	redirect = parse_redirect(args, &cmd_end);
	cmd = prepare_command(args, cmd_end);
	if (!cmd)
	{
		free_string_array(args);
		return (1);
	}
	// リダイレクションの設定
	if (redirect && !setup_redirection(redirect))
	{
		free_redirect(redirect);
		if (cmd != args)
			free_string_array(cmd);
		free_string_array(args);
		return (1);
	}
	// コマンドの実行
	if (is_builtin(cmd[0]))
		status = execute_builtin(cmd);
	else
		status = execute_external_command(cmd, envp);
	// リダイレクションの復元と解放
	if (redirect)
	{
		restore_redirection(redirect);
		free_redirect(redirect);
	}
	if (cmd != args)
		free_string_array(cmd);
	free_string_array(args);
	return (status);
}
