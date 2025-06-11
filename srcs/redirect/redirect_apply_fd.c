/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_apply_fd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:32:38 by muiida            #+#    #+#             */
/*   Updated: 2025/06/11 11:33:22 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

void	apply_output_redirection_fd(t_redirect *redirect, int fd)
{
	(void)redirect;
	if (fd == -1)
		perror("minishell: apply_output_redirection_fd");
	close(fd);
}

void	apply_input_redirection_fd(int fd)
{
	if (fd == -1)
		perror("minishell: apply_input_redirection_fd");
	close(fd);
}
