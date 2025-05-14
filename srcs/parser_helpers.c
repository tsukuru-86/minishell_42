/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 02:30:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/15 02:30:21 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 累積された引数を最後のコマンドに追加 */
bool	handle_final_arg(t_command **cmd, char *accumulated_arg)
{
	if (accumulated_arg && *accumulated_arg)
	{
		if (!add_argument(*cmd, accumulated_arg))
		{
			free(accumulated_arg);
			return (false);
		}
	}
	if (accumulated_arg)
		free(accumulated_arg);
	return (true);
}
