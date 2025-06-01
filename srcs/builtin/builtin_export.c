/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/02 03:49:15 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"

/* export組み込みコマンド */
int	builtin_export(char **args)
{
	int	i;
	int	ret;

	if (!args[1])
	{
		print_sorted_env(1);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		ret = process_export_with_reconstruction(args, &i);
		if (ret)
			return (ret);
	}
	return (0);
}
