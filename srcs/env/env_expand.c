/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 14:56:05 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/26 03:55:26 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

/* 文字が環境変数の開始を示すかどうかをチェック */
static int	is_env_var_start(const char *str, int i)
{
	if (str[i] == '$' && str[i + 1])
		if ((ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'))
			return (1);
	return (0);
}

/* Append quoted section starting at str[i] */
static void	append_quoted(const char *str, int *i, char *res, int *j)
{
	res[(*j)++] = str[(*i)++];
	while (str[*i] && str[*i] != '\'')
		res[(*j)++] = str[(*i)++];
	if (str[*i])
		res[(*j)++] = str[(*i)++];
}

/* Expand environment variables in string */
static char	*expand_env_vars_core(const char *input_str, int in_dquote)
{
	int		i;
	int		j;
	char	res[4096];

	i = 0;
	j = 0;
	while (input_str[i])
	{
		if (input_str[i] == '\'' && !in_dquote)
			append_quoted(input_str, &i, res, &j);
		else if (is_env_var_start(input_str, i))
		{
			if (append_env(input_str, &i, res, &j) < 0)
				return (NULL);
		}
		else
			res[j++] = input_str[i++];
	}
	res[j] = '\0';
	return (ft_strdup(res));
}

/* 環境変数展開のメイン関数 */
char	*expand_env_vars(const char *input_str, int in_dquote)
{
	char	*expanded;

	if (!input_str)
		return (NULL);
	expanded = expand_env_vars_core(input_str, in_dquote);
	if (!expanded)
		return (NULL);
	return (expanded);
}
