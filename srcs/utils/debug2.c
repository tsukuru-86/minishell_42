/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:49:23 by muiida            #+#    #+#             */
/*   Updated: 2025/06/25 21:51:29 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	debug_print_with_int(const char *prefix, int value)
{
	if (DEBUG_ENABLED)
		ft_printf_fd(STDERR_FILENO, "%s%d\n", prefix, value);
}
