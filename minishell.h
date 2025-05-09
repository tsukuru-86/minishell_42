/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-25 04:10:30 by tsukuru           #+#    #+#             */
/*   Updated: 2025-04-25 04:10:30 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <limits.h>
# include <signal.h>
# include "libft/libft.h"

# define MAX_TOKENS 1024

/* Redirection types */
# define REDIR_OUT    1  /* > */
# define REDIR_IN     2  /* < */
# define REDIR_APPEND 3  /* >> */

/* Redirection structure */
typedef struct s_redirect
{
    int     type;           /* リダイレクションのタイプ */
    char    *file;          /* リダイレクト先のファイル名 */
    int     original_fd;    /* 元のファイルディスクリプタを保存 */
} t_redirect;

void    free_tokens(char **tokens);
char    **split_command(char *input);
int     excute_commands(char **args, char **envp);

/* Built-in commands */
int     builtin_echo(char **args);
int     builtin_cd(char **args);
int     builtin_pwd(char **args);
int     builtin_export(char **args);
int     builtin_unset(char **args);
int     builtin_env(char **args);
int     builtin_exit(char **args);

/* External commands */
char    *find_command(char *cmd, char **envp);
int     execute_external_command(char **args, char **envp);

/* Redirection functions */
t_redirect   *create_redirect(int type, char *file);
int          setup_redirection(t_redirect *redirect);
void         restore_redirection(t_redirect *redirect);
void         free_redirect(t_redirect *redirect);

#endif