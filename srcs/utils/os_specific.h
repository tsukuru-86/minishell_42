/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   os_specific.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 11:15:10 by muiida            #+#    #+#             */
/*   Updated: 2025/06/20 16:59:51 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OS_SPECIFIC_H
# define OS_SPECIFIC_H

/*
 * このヘッダーは__APPLE__とLinux（Ubuntu）間での関数の違いを吸収します
 * MakefileでOS___APPLE__またはOS_LINUXが定義されていることを前提としています
 */

# ifdef __APPLE__
/* __APPLE__固有の関数宣言 - 一部の関数が__APPLE__では明示的に宣言が必要 */
void	rl_replace_line(const char *text, int clear_undo);
void	rl_clear_history(void);
# endif

#endif /* OS_SPECIFIC_H */
