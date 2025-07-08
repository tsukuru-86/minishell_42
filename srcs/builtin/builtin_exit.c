/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:52:15 by muiida            #+#    #+#             */
/*   Updated: 2025/06/07 03:13:30 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"
#include <unistd.h>

/* 数値エラーを処理して終了 */
static void	exit_with_numeric_error(const char *arg)
{
	ft_printf_fd(STDERR_FILENO, "exit: %s: numeric argument required\n", arg);
	exit(255);
}

int	builtin_exit(char **args)
{
	unsigned char	status;
	char			*arg;

	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!args[1])
		exit(0);
	
	// 引数が2つ以上ある場合のチェックを先に行う
	if (args[1] && args[2])
	{
		// 第一引数が数値でない場合は数値エラーで終了
		if (!is_valid_number(args[1]))
			exit_with_numeric_error(args[1]);
		// 数値の場合は引数が多すぎるエラーを返すが終了しない
		ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	
	arg = args[1];
	
	// 数値チェック
	if (!is_valid_number(arg))
		exit_with_numeric_error(args[1]);
	
	status = (unsigned char)parse_exit_number(arg);
	exit(status);
}