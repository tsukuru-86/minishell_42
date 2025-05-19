/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 14:56:05 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/23 00:55:28 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Expand environment variables in string */
static char	*expand_env_vars_core(const char *str, int in_dquote,
		t_command *cmd)
{
	int		i;
	int		j;
	char	res[4096];

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_dquote)
			append_quoted(str, &i, res, &j);
		else if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) || str[i
				+ 1] == '_' || str[i + 1] == '?'))
		{
			if (append_env_str(str, &i, res, &j, cmd) < 0)
				return (NULL);
		}
		else
			res[j++] = str[i++];
	}
	res[j] = '\0';
	return (ft_strdup(res));
}

/* 環境変数展開のメイン関数 */
char	*expand_env_vars(const char *str, int in_dquote, t_command *cmd)
{
	char	*expanded;

	if (!str)
		return (NULL);
	expanded = expand_env_vars_core(str, in_dquote, cmd);
	if (!expanded)
		return (NULL);
	return (expanded);
}
