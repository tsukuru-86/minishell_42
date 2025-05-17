/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 03:56:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/16 15:58:53 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 展開処理のクリーンアップ */
char	*cleanup_expand(t_expand *exp, char *result)
{
	if (exp)
	{
		if (exp->res)
			free(exp->res);
		free(exp);
	}
	return (result);
}
