/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/22 16:17:37 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_COMMANDS_H
# define BUILTIN_COMMANDS_H

# include "../../minishell.h"

/* 関数ポインター型の定義 */
typedef int		(*t_builtin_func)(char **args);

/* ビルトインコマンド */
t_builtin_func	get_builtin_funcs(char *name);
int				builtin_unset(char **args);
int				builtin_export(char **args);
int				remove_env_var(const char *name);
void			print_env_list(t_env *head);
int				process_export_with_reconstruction(char **args, int *i);
int				builtin_export_sort(t_env **env_list);
int				builtin_echo(char **args);
int				builtin_cd(char **args);
int				builtin_pwd(char **args);
int				builtin_exit(char **args);
int				is_valid_identifier(const char *str);
int				builtin_env(char **args);
t_env			*sort_env_list_copy(t_env *list_head);
void			print_sorted_env(int format);

/* exit utils */
int				is_valid_number(const char *str);
int				parse_exit_number(const char *str);
int				is_special_case(char **args);
char			*combine_special_args(char **args);
void			exit_with_numeric_error(const char *arg);
int				is_space(char c);
int				is_overflow(unsigned long long num, int digit);
long long		ft_atoll_safe(const char *str, int *overflow);
int				is_numeric_string(const char *str);
void			put_exit_error(const char *msg, const char *arg);

#endif
