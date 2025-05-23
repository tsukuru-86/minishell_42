/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 14:56:05 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/24 05:43:43 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Append quoted section starting at str[*i] */
void	append_quoted(const char *str, int *i, char *res, int *j)
{
	res[(*j)++] = str[(*i)++];
	while (str[*i] && str[*i] != '\'')
		res[(*j)++] = str[(*i)++];
	if (str[*i])
		res[(*j)++] = str[(*i)++];
}

/* 終了ステータス文字列を結果バッファに追加する */
static int	append_exit_status(int *i, char *res, int *j, t_command *cmd)
{
	char	*status_str;

	(*i)++;
	status_str = ft_itoa(get_exit_status(cmd));
	if (!status_str)
		return (-1);
	ft_strlcpy(res + *j, status_str, 4096 - *j);
	*j += ft_strlen(status_str);
	free(status_str);
	return (0);
}

/* Extract environment variable name from string */
static char	*extract_env_name(const char *str)
{
	int		i;
	char	name[1024];

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		name[i] = str[i];
		i++;
	}
	name[i] = '\0';
	return (ft_strdup(name));
}

/* 通常の環境変数の値を結果バッファに追加する */
static int	append_normal_env(const char *str, int *i, char *res, int *j,
		t_command *cmd)
{
	char	*name;
	char	*value;

	name = extract_env_name(str + *i);
	if (!name)
	{
		ft_putstr_fd((char *)"minishell: memory allocation error\n", 2);
		return (-1);
	}
	value = expand_env_var(name, cmd);
	free(name);
	if (!value)
	{
		return (-1);
	}
	ft_strlcpy(res + *j, value, 4096 - *j);
	*j += ft_strlen(value);
	free(value);
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (0);
}

/* Append environment variable section */
int	append_env_str(const char *str, int *i, char *res, int *j, t_command *cmd)
{
	(*i)++;
	if (str[*i] == '?')
	{
		if (append_exit_status(i, res, j, cmd) < 0)
			return (-1);
	}
	else
	{
		if (append_normal_env(str, i, res, j, cmd) < 0)
			return (-1);
	}
	return (0);
}
