/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 02:00:00 by muiida           #+#    #+#             */
/*   Updated: 2025/05/11 02:00:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef REDIRECT_H
# define REDIRECT_H

# include "minishell.h"

/* Save the original file descriptor for later restoration */
int		save_original_fd(t_redirect *redirect);
/* Check file access permissions before opening */
int		check_file_access(t_redirect *redirect);
/* Open the file based on redirection type */
int		open_redirect_file(t_redirect *redirect);
/* Apply the redirection using the file descriptor */
void	apply_redirection(t_redirect *redirect, int fd);
/* Validate all redirections before processing */
int		validate_redirections(t_redirect *redirect);

#endif