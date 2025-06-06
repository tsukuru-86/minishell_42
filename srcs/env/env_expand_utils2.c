/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 03:30:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/26 03:34:23 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

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

/* Handle exit status variable ($?) */
static char	*get_exit_status_value(int *i)
{
	t_env	*env_node;
	char	*value;

	env_node = get_env_node("?");
	if (env_node && env_node->value)
		value = ft_strdup(env_node->value);
	else
		value = ft_strdup("0");
	(*i)++;
	return (value);
}

/* Handle regular environment variable */
static char	*get_regular_env_value(const char *str, int *i)
{
	char	*name;
	char	*value;
	t_env	*env_node;

	name = extract_env_name(str + *i);
	if (!name)
		return (NULL);
	env_node = get_env_node(name);
	if (env_node && env_node->value)
		value = ft_strdup(env_node->value);
	else
		value = ft_strdup("");
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	free(name);
	return (value);
}

/* Append environment variable section */
int	append_env(const char *str, int *i, char *res, int *j)
{
	char	*value;

	(*i)++;
	if (str[*i] == '?')
		value = get_exit_status_value(i);
	else
		value = get_regular_env_value(str, i);
	if (!value)
		return (-1);
	ft_strlcpy(res + *j, value, 4096 - *j);
	*j += ft_strlen(value);
	free(value);
	return (0);
}
