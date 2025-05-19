/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 03:56:00 by muiida           #+#    #+#             */
/*   Updated: 2025/05/12 04:12:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Append quoted section starting at str[*i] */
static void	append_quoted(const char *str, int *i, char *res, int *j)
{
	res[(*j)++] = str[(*i)++];
	while (str[*i] && str[*i] != '\'')
		res[(*j)++] = str[(*i)++];
	if (str[*i])
		res[(*j)++] = str[(*i)++];
}

/* Append environment variable section */
static int	append_env(const char *str, int *i, char *res, int *j, t_command *cmd)
{
	char	*name;
	char	*value;
	char	*status_str;

	(*i)++;
	if (str[*i] == '?')
	{
		status_str = ft_itoa(get_exit_status(cmd));
		if (!status_str)
			return (-1);
		ft_strlcpy(res + *j, status_str, 4096 - *j);
		*j += ft_strlen(status_str);
		free(status_str);
		(*i)++;
		return (0);
	}
	name = extract_env_name(str + *i);
	if (!name)
		return (-1);
	value = expand_env_var(name, cmd);
	free(name);
	if (!value)
		return (-1);
	ft_strlcpy(res + *j, value, 4096 - *j);
	*j += ft_strlen(value);
	free(value);
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (0);
}

/* Expand environment variables in string */
char	*expand_env_vars_core(const char *str, int in_dquote, t_command *cmd)
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
		else if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) ||
				str[i + 1] == '_' || str[i + 1] == '?'))
		{
			if (append_env(str, &i, res, &j, cmd) < 0)
				return (NULL);
		}
		else
			res[j++] = str[i++];
	}
	res[j] = '\0';
	return (ft_strdup(res));
}

/* Extract environment variable name from string */
char	*extract_env_name(const char *str)
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
