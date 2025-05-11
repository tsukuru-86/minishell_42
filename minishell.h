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

/* トークンの種類を定義 */
typedef enum e_token_type
{
    TOKEN_WORD,          // 通常の単語
    TOKEN_SPACE,         // スペース
    TOKEN_SINGLE_QUOTE,  // シングルクォート
    TOKEN_DOUBLE_QUOTE,  // ダブルクォート
    TOKEN_QUOTED_WORD,   // クォートされた単語
    TOKEN_PIPE,          // パイプ |
    TOKEN_REDIR_IN,      // 入力リダイレクト <
    TOKEN_REDIR_OUT,     // 出力リダイレクト >
    TOKEN_REDIR_APPEND,  // 追加リダイレクト >>
    TOKEN_ENV_VAR,       // 環境変数
    TOKEN_END            // 終端
} t_token_type;

/* 環境変数展開関数のプロトタイプ */
char    *expand_env_var(const char *name);
char    *expand_env_vars(char *str, int in_dquote);

/* トークンを表す構造体 */
typedef struct s_token
{
    char *content;
    t_token_type type;
    struct s_token *next;
} t_token;

t_token *tokenize(char *input);
void free_tokens(t_token *tokens);
void print_tokens(t_token *tokens);

/* Environment variable structure */
typedef struct s_env
{
    char            *name;
    char            *value;
    struct s_env    *next;
} t_env;

/* Environment functions */
t_env   *create_env_list(char **envp);
void    free_env_list(t_env *env);
t_env   *get_env_var(t_env *env, const char *name);
int     set_env_var(t_env **env, const char *name, const char *value);
int     remove_env_var(t_env **env, const char *name);

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

int     excute_commands(t_token *tokens, char **envp);

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

/* Global environment variable */
extern t_env *g_env;


/* Redirection functions */
t_redirect   *create_redirect(int type, char *file);
int          setup_redirection(t_redirect *redirect);
void         restore_redirection(t_redirect *redirect);
void         free_redirect(t_redirect *redirect);

#endif