/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_validate_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 05:42:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/23 19:16:22 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

int	is_invalid_input_device(const char *filename)
{
	if (ft_strcmp(filename, "/dev/stdout") == 0)
		return (1);
	if (ft_strcmp(filename, "/dev/stderr") == 0)
		return (1);
	return (0);
}

int	set_input_access_error(t_redirect *current)
{
	ft_memset(current->error_msg, 0, 256);
	ft_strlcpy(current->error_msg, "minishell: ", 256);
	ft_strlcat(current->error_msg, current->file, 256);
	ft_strlcat(current->error_msg, ": ", 256);
	ft_strlcat(current->error_msg, strerror(errno), 256);
	ft_strlcat(current->error_msg, "\n", 256);
	set_env_node("?", "1");
	return (0);
}

int	validate_input_device_and_access(t_redirect *current)
{
	if (is_invalid_input_device(current->file))
	{
		set_env_node("?", "0");
		return (0);
	}
	if (access(current->file, F_OK) != 0)
		return (set_input_access_error(current));
	if (access(current->file, R_OK) != 0)
		return (set_input_access_error(current));
	return (1);
}

int	validate_input_redirect_in(t_redirect *current)
{
	if (is_invalid_input_device(current->file))
	{
		set_env_node("?", "0");
		return (0);
	}
	if (access(current->file, F_OK) != 0)
		return (set_input_access_error(current));
	if (access(current->file, R_OK) != 0)
		return (set_input_access_error(current));
	return (1);
}
