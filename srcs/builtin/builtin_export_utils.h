/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:42:43 by muiida            #+#    #+#             */
/*   Updated: 2025/06/10 13:43:35 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_EXPORT_UTILS_H
# define BUILTIN_EXPORT_UTILS_H

int		append_env_value(const char *name, const char *value);
char	*reconstruct_split_args(char **args, int start, int *next_idx);
int		validate_and_set_env(char *name, char *value);

int		is_valid_identifier(const char *str);
/* Split export argument into name and value */
void	split_export_arg(char *arg, char **name, char **value, int *append);
#endif