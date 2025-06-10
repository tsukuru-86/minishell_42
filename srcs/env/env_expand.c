/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 14:56:05 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/11 06:57:11 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

static int	is_env_var_start(const char *str, int i)
{
	if (str[i] == '$' && str[i + 1])
		if ((ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'))
			return (1);
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

static void	append_quoted_expand_core(const char *str, int *i, char *buf)
{
	int	k;

	(*i)++;
	k = 0;
	process_quote_content(str, i, buf, &k);
	buf[k] = '\0';
}

static void	append_quoted_expand(const char *str, int *i, char *res, int *j)
{
	int		k;
	char	*buf;

	k = 0;
	buf = (char *)malloc(4096);
	if (!buf)
		return ;
	append_quoted_expand_core(str, i, buf);
	k = 0;
	while (buf[k])
		res[(*j)++] = buf[k++];
	if (str[*i])
		(*i)++;
	free(buf);
}

char	*expand_env_vars(const char *input_str, int in_dquote)
{
	int		i;
	int		j;
	char	*res;
	char	*ret;

	(void)in_dquote;
	i = 0;
	j = 0;
	res = (char *)malloc(4096);
	if (!res)
		return (NULL);
	while (input_str[i])
	{
		if ((input_str[i] == '\'' || input_str[i] == '\"') && (i == 0
				|| input_str[i - 1] != '\\'))
			append_quoted_expand(input_str, &i, res, &j);
		else if (is_env_var_start(input_str, i))
			append_env(input_str, &i, res, &j);
		else
			res[j++] = input_str[i++];
	}
	res[j] = '\0';
	ret = ft_strdup(res);
	free(res);
	return (ret);
}
