/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_apply_fd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:32:38 by muiida            #+#    #+#             */
/*   Updated: 2025/06/10 13:32:39 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

void	apply_output_redirection_fd(t_redirect *redirect, int fd)
{
	int	dup_ret;
	int	saved_errno;

	(void)redirect;
	dup_ret = dup2(fd, STDOUT_FILENO);
	saved_errno = errno;
	printf("[DEBUG] apply_output_redirection_fd: fd=%d, ret=%d, errno=%d\n", fd,
		dup_ret, saved_errno);
	if (fd == -1)
		perror("minishell: apply_output_redirection_fd");
	close(fd);
}

void	apply_input_redirection_fd(t_redirect *redirect, int fd)
{
	int dup_ret;
	int saved_errno;

	dup_ret = dup2(fd, STDIN_FILENO);
	saved_errno = errno;
	printf("[DEBUG] apply_input_redirection_fd: fd=%d, ret=%d, errno=%d\n", fd,
		dup_ret, saved_errno);
	if (redirect->type == REDIR_HEREDOC && redirect->file)
		unlink(redirect->file);
	if (fd == -1)
		perror("minishell: apply_input_redirection_fd");
	close(fd);
}