/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils_linux.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:00:07 by muiida            #+#    #+#             */
/*   Updated: 2025/06/20 20:02:34 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

/* Linux専用の実装 */
HIST_ENTRY	**get_history_entries(void)
{
	return (history_list());
}
