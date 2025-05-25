/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 03:22:41 by muiida            #+#    #+#             */
/*   Updated: 2025/05/26 03:54:42 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H
# include "minishell.h"

/* linked-list (node) */
int		remove_env_node(const char *name);
t_env	*get_env_node(const char *name);

/* sorting  */
t_env	*swap_nodes_in_list(t_env *list_head, t_env *prev, t_env *node1,
			t_env *node2);
t_env	*sort_single_pass(t_env *list_head, int *swapped_flag);
t_env	*sort_env_list_copy(t_env *list_head);

/* Variables expansion */
char	*expand_env_node(const char *name);
char	*expand_env_var(const char *name);
char	*expand_env_vars(const char *str, int in_dquote);
int		append_env(const char *str, int *i, char *res, int *j);
/* display */
int		display_all_env_vars(int fd);

#endif