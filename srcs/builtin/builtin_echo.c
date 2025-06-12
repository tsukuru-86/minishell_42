/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/12 15:46:56 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** echoコマンドの-nオプション判定
*/
static int	is_echo_n_option(const char *s)
{
	int	i;

	if (!s || s[0] != '-' || s[1] != 'n')
		return (0);
	i = 2;
	while (s[i] == 'n')
		i++;
	if (s[i] != '\0')
		return (0);
	return (1);
}

/*
** デバッグ用: トークナイズ直後の引数配列を出力
*/
// static void	debug_print_args(char **args)
// {
// 	int	i;

// 	i = 0;
// 	while (args[i])
// 	{
// 		printf("[%d]=%s\n", i, args[i]);
// 		i++;
// 	}
// }

int	builtin_echo(char **args)
{
	int	newline;
	int	i;
	int	first_arg;

	i = 0;
	newline = 1;
	i = 1;
	while (is_echo_n_option(args[i]))
	{
		newline = 0;
		i++;
	}
	first_arg = i;
	while (args[i])
	{
		if (i > first_arg)
			ft_putchar_fd(' ', 1);
		ft_putstr_fd(args[i], 1);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
	return (0);
}
