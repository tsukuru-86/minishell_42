/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 03:22:41 by muiida            #+#    #+#             */
/*   Updated: 2025/05/28 20:44:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H
# include "minishell.h"

/* Environment */
int		remove_env_node(const char *name);
void	append_quoted(t_env_expand_ctx *ctx);
int		append_env_str(t_env_expand_ctx *ctx);
/* Export sorting utilities */
t_env	*swap_nodes_in_list(t_env *list_head, t_env *prev, t_env *node1,
			t_env *node2);
t_env	*sort_single_pass(t_env *list_head, int *swapped_flag);
t_env	*sort_env_list_copy(t_env *list_head);
char	*expand_env_var(const char *name, t_command *cmd);
char	*expand_env_node(const char *name);
/* Environment variable  */
int		display_all_env_vars(int fd);

#endif
