/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:42:43 by muiida            #+#    #+#             */
/*   Updated: 2025/07/09 02:23:44 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"

typedef struct s_export_arg
{
	char		**name;
	char		**value;
	int			*append;
}				t_export_arg;

int				append_env_value(const char *name, const char *value);
char			*reconstruct_split_args(char **args, int start, int *next_idx);
int				validate_and_set_env(char *name, char *value);
int				set_env_node_direct(const char *name, const char *value);

int				is_valid_identifier(const char *str);
/* Split export argument into name and value */
void			split_export_arg(char *arg, char **name, char **value,
					int *append);

char			*ft_trim_spaces(const char *src);
char			*strip_quotes(char *str);
char			*unquote(const char *src);
char			*trim_and_unquote(const char *src);
void			normalize_export_args(char **name, char **value);
char			*create_new_value(const char *old_value,
					const char *append_value);

void			split_export_arg(char *arg, char **name, char **value,
					int *append);

void			put_exit_error(const char *msg, const char *arg);

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
int				is_valid_identifier(const char *str);
t_env			*sort_env_list_copy(t_env *list_head);
void			print_sorted_env(int format);
void			put_exit_error(const char *msg, const char *arg);
#endif