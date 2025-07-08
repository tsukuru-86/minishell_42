/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_apply_fd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:32:38 by muiida            #+#    #+#             */
/*   Updated: 2025/07/09 02:10:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect.h"

void	apply_output_redirection_fd(t_redirect *redirect, int fd)
{
	(void)redirect;
	if (fd == -1)
	{
		perror("minishell: apply_output_redirection_fd");
		return ;
	}
	close(fd);
}

void	apply_input_redirection_fd(int fd)
{
	if (fd == -1)
	{
		perror("minishell: apply_input_redirection_fd");
		return ;
	}
	close(fd);
}
