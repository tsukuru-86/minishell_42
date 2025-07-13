/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 02:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/14 04:33:55 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_H
# define REDIRECT_H

# include "minishell.h"

int			is_invalid_input_device(const char *filename);
int			set_input_access_error(t_redirect *current);
int			validate_input_device_and_access(t_redirect *current);
int			validate_input_redirect_in(t_redirect *current);

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
/* Apply the redirection using the file descriptor */
void		apply_redirection(t_redirect *redirect, int fd);
/* Validate all redirections before processing */
int			validate_redirections(t_redirect *redirect);
/* Save original file descriptors for later restoration */
void		save_original_fds(t_redirect *redirect);
/* Set up all redirections in the chain */
int			setup_redirection(t_redirect *redirect);
/* Restore all original file descriptors */
void		restore_redirection(t_redirect *redirect);
int			validate_input_redirect(t_redirect *current);

/* Helper functions for redirect processing */
t_redirect	*find_last_output_redirect(t_redirect *redirect);
t_redirect	*find_last_input_redirect(t_redirect *redirect);
int			skip_non_effective_redirects(t_redirect *redirect,
				t_redirect *last_out, t_redirect *last_in);

/* Helper functions for redirect_utils2.c */
char		*get_parent_dir(const char *file, char *dir);
int			check_directory_access(t_redirect *redirect);
#endif
