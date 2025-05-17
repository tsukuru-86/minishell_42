/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 03:56:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/16 15:58:57 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 環境変数名の抽出 */
char	*extract_env_name(const char *str)
{
	int		i;
	char	name[1024];

	i = 0;
	if (str[i] == '?' || str[i] == '$')
	{
		name[0] = str[i];
		name[1] = '\0';
		i = 1;
	}
	else
	{
		while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		{
			name[i] = str[i];
			i++;
		}
		name[i] = '\0';
	}
	return (ft_strdup(name));
}

/* クオート文字の処理 */
void	handle_quote(t_expand *exp)
{
	exp->i++;
	while (exp->str[exp->i] && exp->str[exp->i] != '\'')
	{
		exp->res[exp->j] = exp->str[exp->i];
		exp->i++;
		exp->j++;
	}
	if (exp->str[exp->i] == '\'')
		exp->i++;
}

/* 環境変数セクションの追加 */
int	append_env(t_expand *exp)
{
	char	*name;
	char	*value;

	exp->i++;
	name = extract_env_name(exp->str + exp->i);
	if (!name)
		return (-1);
	value = expand_env_var(name, exp->shell);
	free(name);
	if (!value)
		return (-1);
	ft_strlcpy(exp->res + exp->j, value, 4096 - exp->j);
	exp->j += ft_strlen(value);
	free(value);
	while (exp->str[exp->i] && (ft_isalnum(exp->str[exp->i])
			|| exp->str[exp->i] == '_'))
		exp->i++;
	return (0);
}

/* 展開処理の初期化 */
t_expand	*init_expand(const char *str, bool in_dquote, t_minishell *shell)
{
	t_expand	*exp;

	exp = (t_expand *)ft_calloc(1, sizeof(t_expand));
	if (!exp)
		return (NULL);
	exp->str = str;
	exp->res = (char *)ft_calloc(4096, sizeof(char));
	if (!exp->res)
	{
		free(exp);
		return (NULL);
	}
	exp->i = 0;
	exp->j = 0;
	exp->in_dquote = in_dquote;
	exp->shell = shell;
	return (exp);
}

/* 環境変数の展開 */
char	*expand_env_vars(const char *str, bool in_dquote, t_minishell *shell)
{
	t_expand	*exp;
	char		*result;

	if (!str)
		return (NULL);
	exp = init_expand(str, in_dquote, shell);
	if (!exp)
		return (NULL);
	while (exp->str[exp->i] && exp->j < 4095)
	{
		if (exp->str[exp->i] == '\'' && !exp->in_dquote)
			handle_quote(exp);
		else if (exp->str[exp->i] == '$' && exp->str[exp->i + 1])
			append_env(exp);
		else
			exp->res[exp->j++] = exp->str[exp->i++];
	}
	exp->res[exp->j] = '\0';
	result = ft_strdup(exp->res);
	return (cleanup_expand(exp, result));
}
