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
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <stdio.h>

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
char				*expand_env_node(const char *name);
char				*expand_env_vars(const char *str, int in_dquote);
char				*expand_env_vars_core(const char *str, int in_dquote);

/* トークンを表す構造体 */
typedef struct s_token
{
	char			*content;
	t_token_type	type;
	struct s_token	*next;
}					t_token;

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
int					remove_env_var(const char *name);
int					is_valid_identifier(const char *str);
int					process_export_arg(char *arg);
int					excute_commands(t_command *cmd);

/* Builtin command functions */
int					is_builtin(char *cmd);
int					execute_builtin(char **args);
int					execute_builtin_with_redirect(t_command *cmd);

/* External commands */
char				*find_command(char *cmd);
int					execute_external_command(char **args);
int					execute_external_with_fork(t_command *cmd);
void				execute_child_process(t_command *cmd);

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
/* Environment functions */
t_env				*create_env_list(char **envp);
void				free_env_list(void);
int					remove_env_node(const char *name);

/* Environment utility functions */
t_env				*create_env_node(const char *str);
int					update_env_value(t_env *env_node, const char *value);
int					append_env_node(const char *name, const char *value);
t_env				*get_env_node(const char *name);
int					set_env_node(const char *name, const char *value);

/* Command preparation functions */
void				pipeline_init(t_command *cmd);
void				external_command(void);

/* --- Pipeline --- */
int					setup_pipeline(t_command *cmd);
void				cleanup_pipeline(t_command *cmd);
int					wait_pipeline(t_command *cmd);
int					spawn_pipeline_processes(t_command *cmd);
void				init_pipeline(t_command *cmd);
int					create_pipes(t_command *cmd);
void				close_parent_pipes(t_command *cmd);
void				pipeline_close_pipes(t_command *cmd_list);
void				setup_pipeline_child_env(void);


/* --- Tokenizer --- */
t_token				*tokenize(char *input);
t_token				*create_token(char *content, t_token_type type);
void				add_token_to_list(t_token **token_list_head,
						t_token *new_token);
int					is_delimiter(char c);
int					is_quote(char c);
int					is_meta(char c);
void				free_tokens(t_token *tokens);
t_token				*create_meta_token(char *input, int *i);
int					process_token_segment(char *input, int *i,
						t_token **tokens);
int					extract_quoted_string(char *input, int *i, char *word_buf,
						t_token_type *type);

/* --- Environment --- */
t_env				**g_env(void);
void				free_env_list(void);
t_env				*get_env_node(const char *name);
int					set_env_node(const char *name, const char *value);
int					remove_env_var(const char *name);
int					is_valid_identifier(const char *name);
int					update_env_value(t_env *node, const char *value);
t_env				*create_env_node(const char *env_str);
char				*expand_env_vars(const char *str, int in_dquote);
char				*expand_env_node(const char *name);

/* --- External Commands --- */
int					execute_external_command(char **args);
char				*find_command_path(char *cmd);
void				launch_child(char *cmd_path, char **args);
int					launch_parent(pid_t pid, char *cmd_path);
void				free_env_array(char **env_array, int count);
int					count_env_nodes(t_env *env_list);
char				**env_list_to_array(t_env *env_list);

/* --- Builtins --- */
int					execute_builtin(char **args);

/* --- Redirection --- */
int					setup_redirection(t_redirect *redirects);
void				restore_redirection(t_redirect *redirects);

#endif
