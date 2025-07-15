/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   os_specific.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 11:15:10 by muiida            #+#    #+#             */
/*   Updated: 2025/07/15 18:32:41 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OS_SPECIFIC_H
# define OS_SPECIFIC_H
# include <readline/history.h>

# ifdef __APPLE__
#  include <sys/syslimits.h>

void		rl_replace_line(const char *text, int clear_undo);
void		rl_clear_history(void);
void		rl_delete_text(int start, int end);
int			rl_insert_text(const char *text);
int			rl_delete(int count, int key);
int			rl_insert(int c, int count);
HIST_ENTRY	*history_get(int index);
# endif
#endif /* OS_SPECIFIC_H */
