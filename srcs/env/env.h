/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 03:22:41 by muiida            #+#    #+#             */
/*   Updated: 2025/07/24 02:46:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H
# include "minishell.h"

/* Quote processing structure */
typedef struct s_quote_info
{
	int		quote;
	int		dollar;
	char	*buf;
	int		*k;
}			t_quote_info;

/* sorting  */
t_env		*sort_single_pass(t_env *list_head, int *swapped_flag);

/* Variables expansion */
char		*expand_env_node(const char *name);
char		*expand_env_var(const char *name);
int			append_env(const char *str, int *i, char *res, int *j);
void		append_c_escaped(const char *str, int *i, char *buf, int *k);
void		process_quote_content(const char *str, int *i, char *buf, int *k);
/* display */
int			display_all_env_vars(int fd);
char		*expand_env_vars(const char *input_str, int in_dquote);
char		*expand_redirect_filename(const char *filename);
#endif
