/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 14:56:05 by tsukuru           #+#    #+#             */
/*   Updated: 2025/07/09 02:30:06 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

int	is_env_var_start(const char *str, int i)
{
	if (str[i] == '$' && str[i + 1])
	{
		if (ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?')
			return (1);
		if (str[i + 1] == '"' || str[i + 1] == '\'')
			return (2);
	}
	return (0);
}

void	append_c_escaped(const char *str, int *i, char *buf, int *k)
{
	if (str[*i] == 'n')
		buf[(*k)++] = '\n';
	else if (str[*i] == 't')
		buf[(*k)++] = '\t';
	else if (str[*i] == 'r')
		buf[(*k)++] = '\r';
	else if (str[*i] == '\\')
		buf[(*k)++] = '\\';
	else if (str[*i] == '\'')
		buf[(*k)++] = '\'';
	else if (str[*i] == '\"')
		buf[(*k)++] = '\"';
	else if (str[*i])
		buf[(*k)++] = str[*i];
}

static void	process_expansion(const char *input_str, int *i, char *res, int *j)
{
	int	env_check;

	env_check = is_env_var_start(input_str, *i);
	if (env_check == 1)
		append_env(input_str, i, res, j);
	else if (env_check == 2)
		(*i)++;
	else
		res[(*j)++] = input_str[(*i)++];
}

char	*expand_env_vars(const char *input_str, int expand)
{
	int		i;
	int		j;
	char	*res;
	char	*ret;

	i = 0;
	j = 0;
	res = (char *)malloc(4096);
	if (!res)
		return (NULL);
	while (input_str[i] && j < 4095)
	{
		if (expand)
			process_expansion(input_str, &i, res, &j);
		else
			res[j++] = input_str[i++];
	}
	res[j] = '\0';
	ret = ft_strdup(res);
	free(res);
	return (ret);
}

char	*expand_redirect_filename(const char *filename)
{
	int		i;
	int		j;
	int		env_check;
	char	*res;
	char	*ret;

	i = 0;
	j = 0;
	res = (char *)malloc(4096);
	if (!res)
		return (NULL);
	while (filename[i] && j < 4095)
	{
		env_check = is_env_var_start(filename, i);
		if (env_check == 1)
			append_env(filename, &i, res, &j);
		else if (env_check == 2)
			i++;
		else
			res[j++] = filename[i++];
	}
	res[j] = '\0';
	ret = ft_strdup(res);
	free(res);
	return (ret);
}
