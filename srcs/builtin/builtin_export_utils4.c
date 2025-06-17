/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils4.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 07:20:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/17 20:10:51 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_export.h"
#include "minishell.h"

static char	*join_with_space(char *result, char *arg)
{
	char	*temp;
	char	*new_result;

	temp = ft_strjoin(result, " ");
	free(result);
	if (!temp)
		return (NULL);
	new_result = ft_strjoin(temp, arg);
	free(temp);
	return (new_result);
}

char	*reconstruct_split_args(char **args, int start, int *next_idx)
{
	char	*result;
	int		i;

	if (!args[start])
		return (NULL);
	if (!ft_strchr(args[start], '=') && !ft_strchr(args[start], '+'))
		return (NULL);
	result = ft_strdup(args[start]);
	if (!result)
		return (NULL);
	i = start + 1;
	while (args[i] && !ft_strchr(args[i], '=') && !is_valid_identifier(args[i]))
	{
		result = join_with_space(result, args[i]);
		if (!result)
			return (NULL);
		i++;
	}
	*next_idx = i;
	return (result);
}
