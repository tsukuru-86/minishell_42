/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/11 12:10:29 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../error/error_messages.h"
#include "external.h"
#include "minishell.h"

static int	is_env_present(char **env_array, char *name)
{
	int	i;
	int	k;

	i = 0;
	while (env_array && env_array[i])
	{
		k = 0;
		while (name[k] && env_array[i][k] && name[k] == env_array[i][k])
			k++;
		if (name[k] == '\0' && env_array[i][k] == '=')
			return (1);
		i++;
	}
	return (0);
}

static int	count_env_array(char **env_array)
{
	int	count;

	count = 0;
	while (env_array && env_array[count])
		count++;
	return (count);
}

static char	**add_missing_env_vars(char **env_array, char **vars, int added)
{
	int		count;
	char	**new_env;
	int		i;
	int		x;

	count = count_env_array(env_array);
	new_env = (char **)malloc(sizeof(char *) * (count + added + 1));
	if (!new_env)
		return (env_array);
	i = 0;
	while (i < count)
	{
		new_env[i] = env_array[i];
		i++;
	}
	x = 0;
	while (x < added)
	{
		new_env[i + x] = vars[x];
		x++;
	}
	new_env[i + x] = NULL;
	return (new_env);
}

static char	**ensure_env_vars(char **env_array)
{
	char	*vars[3];
	char	*names[3];
	int		added;
	int		i;

	names[0] = "LC_COLLATE";
	names[1] = "LANG";
	names[2] = "PATH";
	added = 0;
	i = 0;
	while (i < 3)
	{
		if (!is_env_present(env_array, names[i]))
		{
			vars[added] = getenv(names[i]);
			if (vars[added])
				added++;
		}
		i++;
	}
	if (added == 0)
		return (env_array);
	return (add_missing_env_vars(env_array, vars, added));
}

void	launch_child(char *cmd_path, char **args)
{
	char	**env_array;

	env_array = env_list_to_array();
	if (!env_array)
	{
		perror("minishell: env_list_to_array failed");
		free(cmd_path);
		exit(127);
	}
	env_array = ensure_env_vars(env_array);
	execve(cmd_path, args, env_array);
	perror("minishell: execve failed");
	exit(127);
}

/* 外部コマンドを実行する関数。
   コマンドパスを検索し、子プロセスでコマンドを実行する */
