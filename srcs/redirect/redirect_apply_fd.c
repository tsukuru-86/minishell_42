/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_apply_fd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:32:38 by muiida            #+#    #+#             */
/*   Updated: 2025/07/01 23:05:33 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

void	apply_output_redirection_fd(t_redirect *redirect, int fd)
{
	debug_print_with_int(
		"[DEBUG] apply_output_redirection_fd: Start, fd: ", fd);
	(void)redirect;
	if (fd == -1)
	{
		perror("minishell: apply_output_redirection_fd");
		debug_print(
			"[DEBUG] apply_output_redirection_fd: fd is -1, returning.");
		return ;
	}
	debug_print_with_int(
		"[DEBUG] apply_output_redirection_fd: Closing fd: ", fd);
	close(fd);
	debug_print("[DEBUG] apply_output_redirection_fd: Exit.");
}

void	apply_input_redirection_fd(int fd)
{
	debug_print_with_int(
		"[DEBUG] apply_input_redirection_fd: Start, fd: ", fd);
	if (fd == -1)
	{
		perror("minishell: apply_input_redirection_fd");
		debug_print(
			"[DEBUG] apply_input_redirection_fd: fd is -1, returning.");
		return ;
	}
	debug_print_with_int(
		"[DEBUG] apply_input_redirection_fd: Closing fd: ", fd);
	close(fd);
	debug_print("[DEBUG] apply_input_redirection_fd: Exit.");
}

