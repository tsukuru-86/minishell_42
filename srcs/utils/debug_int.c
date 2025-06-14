/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 07:22:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/15 07:22:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	debug_print_with_int(const char *prefix, int value, bool is_debug)
{
	if (is_debug)
		printf("%s%d\n", prefix, value);
}
