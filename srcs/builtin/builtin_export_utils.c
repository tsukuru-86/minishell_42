/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 01:15:47 by muiida           #+#    #+#             */
/*   Updated: 2025/05/24 00:00:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "builtin_export.h"
#include "error/error_messages.h"
#include "minishell.h"

static int	process_export_arg(char *arg)
{
	char	*name;
	char	*value;
	int		append;
	int		ret;

	split_export_arg(arg, &name, &value, &append);
	if (append)
		ret = append_env_value(name, value);
	else
		ret = validate_and_set_env(name, value);
	free(name);
	free(value);
	return (ret);
}

/* Process export arguments with reconstruction for split cases */
int	process_export_with_reconstruction(char **args, int *i)
{
	char	*reconstructed;
	int		next_idx;
	int		ret;

	if (ft_strchr(args[*i], '=') && ft_strchr(args[*i], '=')[1] == '\0'
		&& args[*i + 1] && !ft_strchr(args[*i + 1], '='))
	{
		reconstructed = reconstruct_split_args(args, *i, &next_idx);
		if (reconstructed)
		{
			ret = process_export_arg(reconstructed);
			free(reconstructed);
			*i = next_idx;
			return (ret);
		}
	}
	ret = process_export_arg(args[*i]);
	(*i)++;
	return (ret);
}
