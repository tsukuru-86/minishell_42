/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_argutils.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:42:49 by muiida            #+#    #+#             */
/*   Updated: 2025/06/10 13:42:50 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_EXPORT_ARGUTILS_H
# define BUILTIN_EXPORT_ARGUTILS_H

char	*trim_spaces(char *str);
char	*strip_quotes(char *str);
void	split_export_arg(char *arg, char **name, char **value, int *append);

#endif