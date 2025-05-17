/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 14:56:05 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/16 15:55:49 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 特殊環境変数（$?と$$）の値を取得 */
char	*expand_special_var(const char *name, t_minishell *shell)
{
	char	*numstr;
	pid_t	pid;

	if (name[0] == '?')
	{
		numstr = ft_itoa(shell->exit_status);
		if (!numstr)
			return (NULL);
		return (numstr);
	}
	else if (name[0] == '$')
	{
		pid = getpid();
		numstr = ft_itoa(pid);
		if (!numstr)
			return (NULL);
		return (numstr);
	}
	return (NULL);
}

/* 環境変数の値を取得 */
char	*expand_env_var(const char *name, t_minishell *shell)
{
	t_env	*env_var;
	char	*value;

	if (!name || !*name)
		return (ft_strdup("$"));
	if (name[0] == '?' || name[0] == '$')
		return (expand_special_var(name, shell));
	env_var = get_env_var(shell->env, name);
	if (!env_var || !env_var->value)
		return (ft_strdup(""));
	value = ft_strdup(env_var->value);
	if (!value)
		return (NULL);
	return (value);
}
