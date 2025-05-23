/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/24 05:06:52 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"

/* エラーメッセージを出力するヘルパー関数 */
static void	print_unset_invalid_identifier_error(const char *identifier)
{
	ft_putstr_fd((char *)"unset: '", 2);
	ft_putstr_fd((char *)identifier, 2);
	ft_putstr_fd((char *)"': not a valid identifier\n", 2);
}

/* 環境変数を削除するビルトインコマンド。
   引数チェックを行い、有効な変数名のみ削除を実行する */
int	builtin_unset(char **args)
{
	int	i;

	if (!args[1])
	{
		ft_putstr_fd((char *)ERR_UNSET_NOT_ENOUGH_ARGS, 2);
		return (1);
	}
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			print_unset_invalid_identifier_error(args[i]);
			return (1);
		}
		remove_env_var(args[i]);
		i++;
	}
	return (0);
}
