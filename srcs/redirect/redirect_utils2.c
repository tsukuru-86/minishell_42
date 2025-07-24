/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 01:31:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/24 22:03:25 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

char	*get_parent_dir(const char *file, char *dir)
{
	int	len;

	len = dir - file;
	if (len == 0)
		return (ft_strdup("/"));
	else
		return (ft_substr(file, 0, len));
}

static int	check_current_dir_access(t_redirect *redirect)
{
	if (access(".", W_OK) != 0)
	{
		ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n", redirect->file,
			strerror(errno));
		set_env_node("?", "1");
		return (-1);
	}
	return (0);
}

static int	check_parent_dir_access(t_redirect *redirect, char *parent_dir)
{
	if (access(parent_dir, F_OK) != 0 || access(parent_dir, W_OK) != 0)
	{
		ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n", redirect->file,
			strerror(errno));
		set_env_node("?", "1");
		free(parent_dir);
		return (-1);
	}
	return (0);
}

int	check_directory_access(t_redirect *redirect)
{
	char	*dir;
	char	*parent_dir;

	dir = ft_strrchr(redirect->file, '/');
	if (dir)
	{
		parent_dir = get_parent_dir(redirect->file, dir);
		if (!parent_dir)
			return (-1);
		if (check_parent_dir_access(redirect, parent_dir) == -1)
			return (-1);
		free(parent_dir);
	}
	else
		return (check_current_dir_access(redirect));
	return (0);
}
