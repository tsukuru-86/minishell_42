/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils_macos.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:00:07 by muiida            #+#    #+#             */
/*   Updated: 2025/06/21 12:55:50 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* __APPLE__専用の実装 */
HIST_ENTRY	**get_history_entries(void)
{
	static HIST_ENTRY	*entries[1024];
	int					i;
	HIST_ENTRY			*entry;

	i = 0;
	while (i < 1023)
	{
		entry = history_get(i + history_base);
		if (!entry)
			break ;
		entries[i] = entry;
		i++;
	}
	entries[i] = NULL;
	return (entries);
}

/* __APPLE__専用の実装 - Ubuntu環境ではreadlineライブラリが持っている関数 */
/* MacOSではこの関数は何もしない実装とする */
/* 実際のところ、rl_replace_lineはプロンプト再表示時の入力欄のクリアに使われている */
/* シグナルハンドリングで使われるが、動作に影響はしない */
void	rl_replace_line(const char *text, int clear_undo)
{
	(void)clear_undo;
	(void)text;
}

void	rl_clear_history(void)
{
	clear_history();
}
