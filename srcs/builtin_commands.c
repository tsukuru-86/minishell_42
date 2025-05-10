/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/11 00:21:31 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	if (args[1] && ft_strncmp(args[1], "-n", 2) == 0)
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

	if (!args[1])
		path = getenv("HOME");
	else
		path = args[1];
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
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
		ft_putstr_fd("pwd: error retrieving current directory\n", 2);
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	return (0);
}

static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

// Convert linked list to array
static t_env	**env_to_array(t_env *env, int count)
{
    t_env *current;
    t_env **arr;
    t_env *temp;
    int count, i, j;

    // 環境変数の数を数える
    count = 0;
    current = env;
    while (current)
    {
        count++;
        current = current->next;
    }

    // 配列に変換
    arr = malloc(sizeof(t_env *) * count);
    if (!arr)
        return;
    current = env;
    for (i = 0; i < count; i++)
    {
        arr[i] = current;
        current = current->next;
    }

    // バブルソート
    for (i = 0; i < count - 1; i++)
    {
        for (j = 0; j < count - 1 - i; j++)
        {
            if (ft_strncmp(arr[j]->name, arr[j + 1]->name, PATH_MAX) > 0)
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    // 出力
    for (i = 0; i < count; i++)
    {
        ft_putstr_fd("declare -x ", 1);
        ft_putstr_fd(arr[i]->name, 1);
        if (arr[i]->value)
        {
            ft_putstr_fd("=\"", 1);
            ft_putstr_fd(arr[i]->value, 1);
            ft_putchar_fd('\"', 1);
        }
        ft_putchar_fd('\n', 1);
    }

    free(arr);
}

int	builtin_export(char **args)
{
	extern t_env	*g_env;
	int				i;
	char			*equal_pos;
	char			*name;
	char			*value;

	if (!args[1])
	{
		print_sorted_env(g_env);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		equal_pos = ft_strchr(args[i], '=');
		if (equal_pos)
		{
			name = ft_substr(args[i], 0, equal_pos - args[i]);
			value = equal_pos + 1;
		}
		else
		{
			name = ft_strdup(args[i]);
			value = NULL;
		}
		if (!is_valid_identifier(name))
		{
			ft_putstr_fd("export: '", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			free(name);
			return (1);
		}
		if (set_env_var(&g_env, name, value) != 0)
		{
			ft_putstr_fd("export: memory allocation error\n", 2);
			free(name);
			return (1);
		}
		free(name);
		i++;
	}
	return (0);
}

int	builtin_unset(char **args)
{
	extern t_env	*g_env;
	int				i;

	if (!args[1])
	{
		ft_putstr_fd("unset: not enough arguments\n", 2);
		return (1);
	}
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("unset: '", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			return (1);
		}
		remove_env_var(&g_env, args[i]);
		i++;
	}
	return (0);
}

int	builtin_env(char **args)
{
	extern char	**environ;
	int			i;

	(void)args;
	i = 0;
	while (environ[i])
	{
		ft_putendl_fd(environ[i], 1);
		i++;
	}
	return (0);
}

int	builtin_exit(char **args)
{
	int	status;

	status = 0;
	if (args[1])
	{
		status = ft_atoi(args[1]);
		if (status < 0 || status > 255)
			status = status % 256;
	}
	exit(status);
	return (status);
}
