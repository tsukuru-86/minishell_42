/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/24 05:06:52 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_MESSAGES_H
# define ERROR_MESSAGES_H

/* ビルトインコマンドのエラーメッセージ */
# define ERR_UNSET_NOT_ENOUGH_ARGS "unset: not enough arguments\n"
# define ERR_UNSET_INVALID_IDENTIFIER "unset: '%s': not a valid identifier\n"
# define ERR_EXPORT_MEMORY_ALLOC "export: memory allocation error\n"
# define ERR_EXPORT_INVALID_IDENTIFIER "export: '%s': not a valid identifier\n"

#endif
