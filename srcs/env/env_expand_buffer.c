/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand_buffer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 04:03:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/04 04:04:07 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

int	is_env_var_start(const char *str, int i)
{
	if (str[i] == '$' && str[i + 1])
		if ((ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'))
			return (1);
	return (0);
}

char	*allocate_expand_buffer(const char *input_str)
{
	size_t	buf_size;
	char	*buf;

	buf_size = ft_strlen(input_str) * 4 + 1;
	buf = malloc(buf_size);
	return (buf);
}

void	handle_normal_char(const char *str, int *i, char *res, int *j)
{
	res[*j] = str[*i];
	(*j)++;
	(*i)++;
}

int	handle_special_char(const char *str, int *i, char *res, int *j)
{
	if (str[*i] == '\'' || is_env_var_start(str, *i))
		return (1);
	handle_normal_char(str, i, res, j);
	return (0);
}
