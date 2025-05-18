/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/19 01:29:43 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 環境変数名が有効な識別子かどうかをチェックする関数。
   先頭が英字かアンダースコアで始まり、その後は英数字とアンダースコアのみを含む必要がある */
int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/* 環境変数を削除するビルトインコマンド。
   引数チェックを行い、有効な変数名のみ削除を実行する */
int	builtin_unset(char **args)
{
	int	i;

	if (!args[1])
	{
		ft_putstr_fd((char *)"unset: not enough arguments\n", 2);
		return (1);
	}
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd((char *)"unset: '", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd((char *)"': not a valid identifier\n", 2);
			return (1);
		}
		remove_env_var(args[i]);
		i++;
	}
	return (0);
}

/* 環境変数の一覧を表示するビルトインコマンド。
   システムの環境変数配列（environ）を順に出力する */
int	builtin_env(char **args)
{
	extern char	**environ;
	int			i;

	(void)args;
	i = 0;
	while (environ[i])
	{
		ft_putendl_fd(environ[i], 1);
		i++;
	}
	return (0);
}
