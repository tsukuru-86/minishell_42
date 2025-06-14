/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:18:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/14 13:56:30 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	read_heredoc_from_pipe(int fd, t_heredoc *heredoc)
{
	char	*content;

	(void)heredoc;
	content = "hello";
	if (!write_heredoc_content(fd, content))
		return (0);
	return (1);
}
