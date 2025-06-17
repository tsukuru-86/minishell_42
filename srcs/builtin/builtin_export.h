/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:42:43 by muiida            #+#    #+#             */
/*   Updated: 2025/06/17 09:07:11 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_EXPORT_H
# define BUILTIN_EXPORT_H

int		append_env_value(const char *name, const char *value);
char	*reconstruct_split_args(char **args, int start, int *next_idx);
int		validate_and_set_env(char *name, char *value);
int		set_env_node_direct(const char *name, const char *value);

int		is_valid_identifier(const char *str);
/* Split export argument into name and value */
void	split_export_arg(char *arg, char **name, char **value, int *append);

char	*ft_trim_spaces(const char *src);
char	*unquote(const char *src);
char	*trim_and_unquote(const char *src);
void	normalize_export_args(char **name, char **value);
char	*create_new_value(const char *old_value, const char *append_value);

void	split_export_arg(char *arg, char **name, char **value, int *append);

#endif