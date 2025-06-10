/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_apply_fd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:32:38 by muiida            #+#    #+#             */
/*   Updated: 2025/06/11 06:25:08 by muiida           ###   ########.fr       */
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

void	apply_input_redirection_fd(int fd)
{
	int			dup_ret;
	int			saved_errno;
	struct stat	st;

	dup_ret = dup2(fd, STDIN_FILENO);
	saved_errno = errno;
	printf("[TRACE] pid=%d, dup2: fd=%d -> STDIN_FILENO, ret=%d, errno=%d\n",
		getpid(), fd, dup_ret, saved_errno);
	{
		if (fstat(STDIN_FILENO, &st) == 0)
			printf("[TRACE] pid=%d, fstat(STDIN_FILENO): mode=%o\n", getpid(),
				st.st_mode);
		else
			printf("[TRACE] pid=%d, fstat(STDIN_FILENO) failed: errno=%d\n",
				getpid(), errno);
	}
	printf("[DEBUG] apply_input_redirection_fd: fd=%d, ret=%d, errno=%d\n", fd,
		dup_ret, saved_errno);
	if (fd == -1)
		perror("minishell: apply_input_redirection_fd");
	close(fd);
}
