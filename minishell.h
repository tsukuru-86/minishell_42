/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 04:10:30 by tsukuru            #+#    #+#             */
/*   Updated: 2025/05/11 22:20:42 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/syslimits.h>
# include <sys/wait.h>
# include <unistd.h>

# define MAX_TOKENS 1024

/* トークンの種類を定義 */
typedef enum e_token_type
{
	TOKEN_WORD,         // 通常の単語
	TOKEN_SPACE,        // スペース
	TOKEN_SINGLE_QUOTE, // シングルクォート
	TOKEN_DOUBLE_QUOTE, // ダブルクォート
	TOKEN_QUOTED_WORD,  // クォートされた単語
	TOKEN_PIPE,         // パイプ |
	TOKEN_REDIR_IN,     // 入力リダイレクト <
	TOKEN_REDIR_OUT,    // 出力リダイレクト >
	TOKEN_REDIR_APPEND, // 追加リダイレクト >>
	TOKEN_ENV_VAR,      // 環境変数
	TOKEN_END           // 終端
}					t_token_type;

void				setup_child_signals(void);

/* 環境変数展開関数のプロトタイプ */
char				*expand_env_var(const char *name);
char				*expand_env_vars(const char *str, int in_dquote);
char				*expand_env_vars_core(const char *str, int in_dquote);
char				*extract_env_name(const char *str);

/* トークンを表す構造体 */
typedef struct s_token
{
	char			*content;
	t_token_type	type;
	struct s_token	*next;
}					t_token;

/* Tokenizer functions */
t_token				*tokenize(char *input);
void				free_tokens(t_token *tokens);
void				print_tokens(t_token *tokens);

int					is_delimiter(char c);
int					is_quote(char c);
int					is_meta(char c);
t_token_type		get_meta_type(char *input, int *i);
t_token				*create_meta_token(char *input, int *i);
t_token				*create_token(char *content, t_token_type type);

/* Redirection structure */
typedef struct s_redirect
{
	int type;                /* リダイレクションのタイプ */
	char *file;              /* リダイレクト先のファイル名 */
	int original_fd;         /* 元のファイルディスクリプタを保存 */
	struct s_redirect *next; /* 次のリダイレクト */
}					t_redirect;

/* パイプライン用の構造体 */
typedef struct s_pipeline
{
	int read_fd;  // 読み込み用FD
	int write_fd; // 書き込み用FD
	pid_t pid;    // プロセスID
}					t_pipeline;

/* コマンド構造体 */
typedef struct s_command
{
	char **args;            // コマンドと引数の配列
	t_redirect *redirects;  // リダイレクトのリスト
	t_pipeline pipe;        // パイプライン情報
	struct s_command *next; // パイプで繋がれた次のコマンド
	struct s_command *prev; // パイプで繋がれた前のコマンド
}					t_command;

/* パイプライン関連の関数プロトタイプ */
int					setup_pipeline(t_command *cmd);
void				cleanup_pipeline(t_command *cmd);
int					wait_pipeline(t_command *cmd);

/* Pipeline helper functions */
void				setup_pipeline_child_env(void);
void				pipeline_close_pipes(t_command *cmd);
void				pipeline_execute_command_logic(t_command *current);

/* Environment variable structure */
typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}					t_env;

/* パーサー関数のプロトタイプ */
t_command			*parse_tokens(t_token *tokens);
void				free_command(t_command *cmd);

/* Environment functions */
t_env				*create_env_list(char **envp);
void				free_env_list(t_env *env);
int					set_env_var(t_env **env, const char *name,
						const char *value);
int					remove_env_var(t_env **env, const char *name);

/* Redirection types */
# define REDIR_OUT 1 /* > */
# define REDIR_IN 2 /* < */
# define REDIR_APPEND 3 /* >> */

char				**split_command(char *input);

/* Built-in commands */
int					builtin_echo(char **args);
int					builtin_cd(char **args);
int					builtin_pwd(char **args);
int					builtin_export(char **args);
int					builtin_unset(char **args);
int					builtin_env(char **args);
int					builtin_exit(char **args);
int					remove_env_var(t_env **env, const char *name);
int					is_valid_identifier(const char *str);
int					process_export_arg(char *arg, t_env **env);
int					excute_commands(t_command *cmd);
int					is_builtin(char *cmd);
int					execute_builtin(char **args);

/* External commands */
char				*find_command(char *cmd);
int					execute_external_command(char **args);

/* Environment array conversion */
char				**env_list_to_array(t_env *env);

/* Global environment variable */
// extern t_env		*g_env;
t_env				**g_env(void);

/* Redirection functions */
t_redirect			*create_redirect(int type, char *file);
int					setup_redirection(t_redirect *redirect);
void				restore_redirection(t_redirect *redirect);
void				free_redirect(t_redirect *redirect);

/* Redirect utility functions */
int					save_original_fd(t_redirect *redirect);
int					open_redirect_file(t_redirect *redirect);
void				apply_redirection(t_redirect *redirect, int fd);

/* Environment utility functions */
t_env				*create_env_node(const char *str);
int					update_env_value(t_env *var, const char *value);
int					append_env_var(t_env **env, const char *name,
						const char *value);
t_env				*get_env_var(t_env *env, const char *name);
int					set_env_var(t_env **env, const char *name,
						const char *value);

/* External command utility functions */
char				*search_in_path(const char *path_env, char *cmd);

/* Redirect parsing functions */
t_redirect			*parse_redirect(char **args, int *cmd_end);
/* Command preparation functions */
char				**prepare_command(char **args, int cmd_end);
t_redirect			*create_redirect(int type, char *file);
int					setup_redirection(t_redirect *redirect);
void				restore_redirection(t_redirect *redirect);
void				free_redirect(t_redirect *redirect);

/* Redirect utility functions */
int					save_original_fd(t_redirect *redirect);
int					open_redirect_file(t_redirect *redirect);
void				apply_redirection(t_redirect *redirect, int fd);

/* Environment utility functions */
t_env				*create_env_node(const char *str);
int					update_env_value(t_env *var, const char *value);
int					append_env_var(t_env **env, const char *name,
						const char *value);
t_env				*get_env_var(t_env *env, const char *name);
int					set_env_var(t_env **env, const char *name,
						const char *value);

/* External command utility functions */
char				*search_in_path(const char *path_env, char *cmd);

/* Redirect parsing functions */
t_redirect			*parse_redirect(char **args, int *cmd_end);
/* Command preparation functions */
char				**prepare_command(char **args, int cmd_end);

#endif
