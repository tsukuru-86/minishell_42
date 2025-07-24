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

#include "libft/libft.h"
#include "minishell.h"
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

/* Search for a command in PATH environment */
char	*search_in_path(const char *path_env, char *cmd)
{
	char	*path;
	char	*dir;
	char	full_path[PATH_MAX];
	char	*saveptr;

	path = ft_strdup(path_env);
	if (!path)
		return (NULL);
	dir = ft_strtok_r(path, ":", &saveptr);
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
		dir = ft_strtok_r(NULL, ":", &saveptr);
	}
	free(path);
	return (NULL);
}

/* Set up signal handling in child process.
   In child, set SIGINT, SIGQUIT, SIGPIPE to default handlers */
void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
}
