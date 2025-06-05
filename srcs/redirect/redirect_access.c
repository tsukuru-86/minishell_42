/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_access.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 00:55:00 by muiida           #+#    #+#             */
/*   Updated: 2025/06/04 00:55:00 by muiida           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"
#include <errno.h>

static int	check_read_perm(char *file)
{
	if (access(file, F_OK) != 0)
	{
		ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n", file,
			strerror(errno));
		return (-1);
	}
	if (access(file, R_OK) != 0)
	{
		ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n", file,
			strerror(errno));
		return (-1);
	}
	return (0);
}

static int	check_dir_write_perm(char *file)
{
	char	*dir_path;
	char	*last_slash;
	int		result;

	dir_path = ft_strdup(file);
	if (!dir_path)
		return (-1);
	last_slash = ft_strrchr(dir_path, '/');
	if (last_slash != NULL)
		*last_slash = '\0';
	else
	{
		free(dir_path);
		dir_path = ft_strdup(".");
		if (!dir_path)
			return (-1);
	}
	result = access(dir_path, W_OK);
	free(dir_path);
	if (result != 0)
	{
		ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n", file,
			strerror(errno));
		return (-1);
	}
	return (0);
}

static int	check_write_perm(char *file)
{
	if (access(file, F_OK) == 0)
	{
		if (access(file, W_OK) != 0)
		{
			ft_printf_fd(STDERR_FILENO, "minishell: %s: %s\n", file,
				strerror(errno));
			return (-1);
		}
		return (0);
	}
	return (check_dir_write_perm(file));
}

int	check_file_access(t_redirect *redirect)
{
	if (!redirect || !redirect->file)
		return (-1);
	if (redirect->type == REDIR_IN)
		return (check_read_perm(redirect->file));
	if (redirect->type == REDIR_OUT || redirect->type == REDIR_APPEND)
		return (check_write_perm(redirect->file));
	return (0);
}

// /* Check file access permissions before opening */
// int	check_file_access(t_redirect *redirect)
// {
// 	if (redirect->type == REDIR_IN)
// 	{
// 		if (access(redirect->file, R_OK) != 0)
// 		{
// 			ft_printf_fd(STDERR_FILENO, "minishell: %s: %s: \n", redirect->file,
// 				redirect->file, strerror(errno));
// 			return (-1);
// 		}
// 		return (0);
// 	}
// 	if (access(redirect->file, F_OK) == 0)
// 	{
// 		if (access(redirect->file, W_OK) != 0)
// 		{
// 			ft_printf_fd(STDERR_FILENO, "minishell: %s: %s: \n", redirect->file,
// 				redirect->file, strerror(errno));
// 			return (-1);
// 		}
// 	}
// 	else if (access(".", W_OK) != 0)
// 	{
// 		perror("minishell: .");
// 		return (-1);
// 	}
// 	return (0);
// }