/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_limits2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 00:08:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/24 00:08:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Check the total size limit of the argument array */
int	check_args_limit(char **args)
{
	size_t	total_size;
	int		i;

	if (!args)
		return (1);
	total_size = 0;
	i = 0;
	while (args[i])
	{
		total_size += ft_strlen(args[i]) + 1;
		if (total_size > ARG_MAX)
		{
			ft_putstr_fd("minishell: argument list too long\n", STDERR_FILENO);
			return (0);
		}
		i++;
	}
	return (1);
}

/* Check the pipe buffer size limit */
int	check_pipe_buffer_limit(size_t size)
{
	if (size > PIPE_BUFFER_SIZE)
	{
		ft_putstr_fd("minishell: pipe buffer overflow\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}
