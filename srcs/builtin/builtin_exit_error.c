/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_error.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 20:10:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/09 02:19:58 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	put_exit_error(const char *msg, const char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	if (arg)
		ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd((char *)msg, 2);
}
