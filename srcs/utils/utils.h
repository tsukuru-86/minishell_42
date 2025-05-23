/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/24 05:33:31 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "../../minishell.h"
# include "../error/error_messages.h"

/* export関連のユーティリティ関数 */
void	split_export_arg(char *arg, char **name, char **value);
int		validate_and_set_env(char *name, char *value);
int		process_export_arg(char *arg);
/* 環境変数表示機能 */
int		display_all_env_vars(int fd);

#endif
