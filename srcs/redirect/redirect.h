/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 02:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/22 01:32:30 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_H
# define REDIRECT_H

# include "../../minishell.h"

/* Internal redirect functions */
void		apply_output_redirection_fd(t_redirect *redirect, int fd);
void		apply_input_redirection_fd(int fd);
int			save_original_fd(t_redirect *redirect);
int			check_file_access(t_redirect *redirect);
int			open_redirect_file(t_redirect *redirect);
void		apply_redirection(t_redirect *redirect, int fd);
int			validate_redirections(t_redirect *redirect);
void		save_original_fds(t_redirect *redirect);
int			process_redirections(t_redirect *redirect);
int			validate_input_redirect(t_redirect *current);

void		apply_output_redirection_fd(t_redirect *redirect, int fd);
void		apply_input_redirection_fd(int fd);

/* Save the original file descriptor for later restoration */
int			save_original_fd(t_redirect *redirect);
/* Check file access permissions before opening */
int			check_file_access(t_redirect *redirect);
/* Open the file based on redirection type */
int			open_redirect_file(t_redirect *redirect);
/* Apply the redirection using the file descriptor */
void		apply_redirection(t_redirect *redirect, int fd);
/* Validate all redirections before processing */
int			validate_redirections(t_redirect *redirect);
/* Save original file descriptors for later restoration */
void		save_original_fds(t_redirect *redirect);
/* Process redirections in order, applying only the last effective one */
int			process_redirections(t_redirect *redirect);
/* Set up all redirections in the chain */
int			setup_redirection(t_redirect *redirect);
/* Restore all original file descriptors */
void		restore_redirection(t_redirect *redirect);
int			validate_input_redirect(t_redirect *current);

/* Helper functions for redirect processing */
t_redirect	*find_last_output_redirect(t_redirect *redirect);
t_redirect	*find_last_input_redirect(t_redirect *redirect);
int			process_non_effective_redirects(t_redirect *redirect,
				t_redirect *last_out, t_redirect *last_in);

/* Helper functions for redirect_utils2.c */
char		*get_parent_dir(const char *file, char *dir);
int			check_directory_access(t_redirect *redirect);
#endif
