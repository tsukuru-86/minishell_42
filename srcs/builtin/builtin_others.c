/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_others.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/01 04:04:20 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	if (args[1] && ft_strlen(args[1]) == 2 && ft_strncmp(args[1], "-n", 2) == 0)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
	return (0);
}

int	builtin_cd(char **args)
{
	char	*path;

	if (args[2] != NULL)
		return (0);
	if (args[1] == NULL)
		path = getenv("HOME");
	else
		path = args[1];
	if (chdir(path) == -1)
	{
		ft_putstr_fd((char *)"cd: ", 2);
		perror(path);
		return (1);
	}
	return (0);
}

int	builtin_pwd(char **args)
{
	char	cwd[PATH_MAX];

	(void)args;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		ft_putstr_fd((char *)"pwd: error retrieving current directory\n", 2);
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	return (0);
}

/* 特殊ケースの処理 */
static int	handle_special_case(char **args)
{
	char	*combined;
	int		status;

	combined = combine_special_args(args);
	if (!is_valid_number(combined))
	{
		exit_with_numeric_error(combined);
		free(combined);
	}
	status = parse_exit_number(combined);
	free(combined);
	return (status);
}

/* 通常ケースの処理 */
static int	handle_normal_case(char **args)
{
	if (!is_valid_number(args[1]))
		exit_with_numeric_error(args[1]);
	return (parse_exit_number(args[1]));
}

int	builtin_exit(char **args)
{
	int	status;

	if (args[1] && args[2] && !is_special_case(args))
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	if (args[1])
	{
		if (is_special_case(args))
			status = (unsigned char)handle_special_case(args);
		else
			status = (unsigned char)handle_normal_case(args);
	}
	else
		status = 0;
	exit(status);
	return (status);
}
