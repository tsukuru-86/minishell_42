/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils4.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 07:20:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/21 12:13:20 by muiida           ###   ########.fr       */
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

static int	is_assignment_operator(char *arg)
{
	if (ft_strcmp(arg, "=") == 0 || ft_strcmp(arg, "+=") == 0)
		return (1);
	if (arg[0] == '=' || arg[0] == '+')
		return (1);
	return (0);
}

char	*reconstruct_split_args(char **args, int start, int *next_idx)
{
	char	*result;
	int		i;

	if (!args[start])
		return (NULL);
	result = ft_strdup(args[start]);
	if (!result)
		return (NULL);
	i = start + 1;
	while (args[i])
	{
		result = join_with_space(result, args[i]);
		if (!result)
			return (NULL);
		i++;
		if (is_assignment_operator(args[i - 1]) || !args[i])
			break ;
		if (is_valid_identifier(args[i]))
			break ;
	}
	*next_idx = i;
	return (result);
}
