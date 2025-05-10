/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 02:10:00 by muiida           #+#    #+#             */
/*   Updated: 2025/05/11 02:10:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <limits.h>
#include <string.h>
#include <unistd.h>

/* Get the PATH environment variable string */
char	*get_path_env(t_env *env)
{
	t_env	*path_env;

	path_env = get_env_var(env, "PATH");
	return (path_env->value);
}

/* Search for a command in PATH environment */
char	*search_in_path(const char *path_env, char *cmd)
{
	char	*path;
	char	*dir;
	char	full_path[PATH_MAX];

	path = ft_strdup(path_env);
	if (!path)
		return (NULL);
	dir = strtok(path, ":");
	while (dir)
	{
		ft_strlcpy(full_path, dir, PATH_MAX);
		ft_strlcat(full_path, "/", PATH_MAX);
		ft_strlcat(full_path, cmd, PATH_MAX);
		if (access(full_path, X_OK) == 0)
		{
			free(path);
			return (ft_strdup(full_path));
		}
		dir = strtok(NULL, ":");
	}
	free(path);
	return (NULL);
}
