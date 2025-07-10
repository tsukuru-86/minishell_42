/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_limits.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 00:07:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/11 06:42:47 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Check the input line length limit */
int	check_input_line_limit(const char *input)
{
	size_t	len;

	if (!input)
		return (0);
	len = ft_strlen(input);
	if (len > ARG_MAX)
	{
		ft_putstr_fd("minishell: input line too long\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

/* Check the filename length limit */
int	check_filename_limit(const char *filename)
{
	char	*basename;
	size_t	len;

	if (!filename)
		return (0);
	len = ft_strlen(filename);
	if (len > PATH_MAX)
	{
		ft_putstr_fd("minishell: filename too long\n", STDERR_FILENO);
		return (0);
	}
	basename = ft_strrchr(filename, '/');
	if (basename)
		basename++;
	else
		basename = (char *)filename;
	if (ft_strlen(basename) > NAME_MAX)
	{
		ft_putstr_fd("minishell: filename too long\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

