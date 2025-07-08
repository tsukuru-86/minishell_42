/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:17:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/09 02:50:04 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

/* 前方宣言 */
# include <limits.h>
# include <unistd.h>

# ifdef __APPLE__
#  include <sys/syslimits.h> // macOS
# else
#  include <linux/limits.h> // Linux
# endif

typedef struct s_token	t_token;

# ifdef __APPLE__
/* __APPLE__固有の関数宣言 - 一部の関数が__APPLE__では明示的に宣言が必要 */
void					rl_replace_line(const char *text, int clear_undo);
void					rl_clear_history(void);
# endif

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
#   define ARG_MAX 2097152
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

char					*prepare_input(char *input);
char					*read_all_pipe_input(void);
int						main_loop(void);
int						handle_empty_input(char *input);
void					process_valid_input(char *input, int *status);
void					free_string_array(char **arr);
void					process_input_lines(char **lines, int *status);
void					process_with_fallback(char *input, int *status);
char					*str_replace_backslash_n(const char *src);
int						handle_tokens_and_parse(t_token *tokens);
void					handle_input(char *input, int *status);
#endif
