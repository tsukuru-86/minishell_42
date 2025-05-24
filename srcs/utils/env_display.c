/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_display.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/24 21:24:48 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_display.h"

/* 環境変数の一覧を表示する機能。
   内部の環境変数リストから値を持つ変数のみを出力する */
int	display_all_env_vars(int fd)
{
	t_env	*current_env;

	current_env = *g_env();
	while (current_env)
	{
		if (current_env->value)
		{
			ft_putstr_fd(current_env->name, fd);
			ft_putstr_fd("=", fd);
			ft_putendl_fd(current_env->value, fd);
		}
		current_env = current_env->next;
	}
	return (0);
}
