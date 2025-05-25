/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/26 00:13:34 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_COMMANDS_H
# define BUILTIN_COMMANDS_H

# include "../../minishell.h"

/* ビルトインコマンド */
int		builtin_env(char **args);
int		builtin_unset(char **args);
int		builtin_export(char **args);
int		builtin_export_new(char **args);
t_env	*sort_env_list_copy(t_env *list_head);
int		remove_env_var(const char *name);
void	print_env_list(t_env *head);
int		set_env_node(const char *name, const char *value);
int		process_export_arg(char *arg);
int		builtin_export_sort(t_env **env_list);
int		builtin_echo(char **args);
int		builtin_cd(char **args);
int		builtin_pwd(char **args);
int		builtin_exit(char **args);

#endif
