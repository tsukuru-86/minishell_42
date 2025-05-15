/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 02:10:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/15 08:38:54 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <limits.h>
#include <string.h>
#include <unistd.h>

static char	*get_cmd_path(const char *dir, char *cmd)
{
	char	*slash;
	char	*full;

	slash = ft_strjoin(dir, "/");
	if (!slash)
		return (NULL);
	full = ft_strjoin(slash, cmd);
	free(slash);
	return (full);
}

static char	*search_in_directory(const char *dir, char *cmd)
{
	char	*full_path;

	full_path = get_cmd_path(dir, cmd);
	if (full_path && access(full_path, X_OK) == 0)
		return (full_path);
	if (full_path)
		free(full_path);
	return (NULL);
}

static char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;

	dup = malloc(n + 1);
	if (!dup)
		return (NULL);
	ft_memcpy(dup, s, n);
	dup[n] = '\0';
	return (dup);
}

/* Search for a command in PATH environment */
char	*search_in_path(const char *path_env, char *cmd)
{
	const char	*start;
	const char	*end;
	char		*dir;
	char		*result;

	start = path_env;
	while (*start)
	{
		end = start;
		while (*end && *end != ':')
			end++;
		dir = ft_strndup(start, end - start);
		if (!dir)
			return (NULL);
		result = search_in_directory(dir, cmd);
		free(dir);
		if (result)
			return (result);
		if (*end == ':')
			start = end + 1;
		else
			start = end;
	}
	return (search_in_directory("", cmd));
}
