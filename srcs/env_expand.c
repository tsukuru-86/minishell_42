/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 14:56:05 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/12 04:10:27 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 環境変数の値を取得 */
char	*expand_env_var(const char *name)
{
	t_env	*env_var;
	char	*value;

	if (!name || !*name)
		return (ft_strdup("$"));
	env_var = get_env_var(g_env, name);
	if (!env_var)
		return (ft_strdup(""));
	value = ft_strdup(env_var->value);
	if (!value)
		return (NULL);
	return (value);
}

/* Expand environment variables in a string */
char	*expand_env_vars(char *str, int in_dquote)
{
	return (expand_env_vars_core(str, in_dquote));
}
