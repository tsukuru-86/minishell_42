/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_limits.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 00:02:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/01 23:00:17 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSTEM_LIMITS_H
# define SYSTEM_LIMITS_H

# include <limits.h>
# include <unistd.h>

/* PATH_MAX: パスの最大長 */
# ifndef PATH_MAX
#  ifdef _POSIX_PATH_MAX
#   define PATH_MAX _POSIX_PATH_MAX
#  else
#   define PATH_MAX 4096
#  endif
# endif

/* ARG_MAX: execve()に渡せる引数の最大バイト数 */
# ifndef ARG_MAX
#  ifdef _POSIX_ARG_MAX
#   define ARG_MAX _POSIX_ARG_MAX
#  else
#   define ARG_MAX 131072
#  endif
# endif

/* NAME_MAX: ファイル名の最大長 */
# ifndef NAME_MAX
#  ifdef _POSIX_NAME_MAX
#   define NAME_MAX _POSIX_NAME_MAX
#  else
#   define NAME_MAX 255
#  endif
# endif

/* PIPE_BUF: パイプバッファのアトミック書き込み保証サイズ */
# ifndef PIPE_BUF
#  ifdef _POSIX_PIPE_BUF
#   define PIPE_BUF _POSIX_PIPE_BUF
#  else
#   define PIPE_BUF 512
#  endif
# endif

/* OPEN_MAX: 同時に開けるファイルディスクリプタの最大数 */
# ifndef OPEN_MAX
#  ifdef _POSIX_OPEN_MAX
#   define OPEN_MAX _POSIX_OPEN_MAX
#  else
#   define OPEN_MAX 256
#  endif
# endif

#endif