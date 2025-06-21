/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_validator.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/24 00:00:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 環境変数名が有効な識別子かどうかをチェックする関数。
   先頭が英字かアンダースコアで始まり、その後は英数字とアンダースコアのみを含む必要がある */
int	is_valid_identifier(const char *str)
{
	int	i;

	if (str == NULL || *str == '\0')
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
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
