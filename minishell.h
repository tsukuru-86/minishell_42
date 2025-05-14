/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 04:10:30 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/15 02:26:45 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/syslimits.h>
# include <sys/wait.h>
# include <unistd.h>

# define MAX_TOKENS 1024
# define RET_ERROR 1
# define RET_SUCCESS 0

/* リダイレクトの種類を定義 */
typedef enum e_redir_type
{
	REDIR_IN,     // 入力リダイレクト <
	REDIR_OUT,    // 出力リダイレクト >
	REDIR_APPEND, // 追加リダイレクト >>
	REDIR_HEREDOC // ヒアドキュメント
}							t_redir_type;

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
	TOKEN_HEREDOC,
	TOKEN_END // 終端
}							t_token_type;

/* 環境変数展開関数のプロトタイプ */
char						*expand_env_var(const char *name);
char						*expand_env_vars(const char *str, int in_dquote);
char						*expand_env_vars_core(const char *str,
								int in_dquote);
char						*extract_env_name(const char *str);

/* トークンを表す構造体 */
typedef struct s_token
{
	char					*content;
	t_token_type			type;
	struct s_token			*next;
}							t_token;

/* Tokenizer functions */
t_token						*tokenize(char *input);
void						free_tokens(t_token *tokens);
void						print_tokens(t_token *tokens);

int							is_delimiter(char c);
int							is_quote(char c);
int							is_meta(char c);
t_token						*create_meta_token(char *input, int *i);
t_token_type				get_meta_type(char *input, int *i);
t_token						*create_token(char *content, t_token_type type);

int							handle_word(char *input, int *i, t_token **tokens,
								char *word_buf);
int							handle_meta_character(char *input, int *i,
								t_token **tokens);
int							handle_quoted_string(char *input, int *i,
								t_token **tokens, char *word_buf);
t_token						*add_token(t_token **token_list,
								const char *content, t_token_type type);
bool						handle_error_token(t_token **current_token_ptr,
								char **accumulated_arg_ptr);
/* Redirection structure */
typedef struct s_redirect
{
	t_redir_type type;       /* リダイレクションのタイプ */
	char *file;              /* リダイレクト先のファイル名 */
	int original_fd;         /* 元のファイルディスクリプタを保存 */
	struct s_redirect *next; /* 次のリダイレクト */
}							t_redirect;

/* パイプライン用の構造体 */
typedef struct s_pipeline
{
	int read_fd;  // 読み込み用FD
	int write_fd; // 書き込み用FD
	pid_t pid;    // プロセスID
}							t_pipeline;

/* コマンド構造体 */
typedef struct s_command
{
	char **args;            // コマンドと引数の配列
	t_redirect *redirects;  // リダイレクトのリスト
	t_pipeline pipe;        // パイプライン情報
	struct s_command *next; // パイプで繋がれた次のコマンド
	struct s_command *prev; // パイプで繋がれた前のコマンド
}							t_command;

/* パイプライン関連 */
int							setup_pipeline(t_command *cmd);
void						cleanup_pipeline(t_command *cmd);
int							wait_pipeline(t_command *cmd);

/* Pipeline helper functions */
void						setup_pipeline_child_env(void);
void						pipeline_close_pipes(t_command *cmd);
void						pipeline_execute_command_logic(t_command *current);
void						pipeline_redirect_io(t_command *current);

/* Environment variable structure */
typedef struct s_env
{
	char					*name;
	char					*value;
	struct s_env			*next;
}							t_env;

typedef struct s_minishell
{
	t_env					*env;
	char					**argv;
	int						argc;
	char					*line;
}							t_minishell;

/* パーサー */
t_command					*parse_tokens(t_token *tokens);
void						free_command(t_command *cmd);
t_command					*create_command(void);
int							add_argument(t_command *cmd, char *arg);
t_command					*initialize_command_list(void);
bool						process_tokens(t_command **cmd, t_token *tokens,
								t_command **head);
bool						process_token_in_parse_loop(t_command **cmd_ptr,
								t_token **current_token_ptr,
								char **accumulated_arg_ptr);

char						**split_command(char *input);

/* Built-in commands */
int							builtin_echo(char **args);
int							builtin_cd(char **args);
int							builtin_pwd(char **args);
int							builtin_export(char **args);
int							builtin_unset(char **args);
int							check_identifier_for_unset(const char *name);
int							validate_identifier_for_removal(const char *name);
t_env						*find_and_remove_node(t_env **env,
								const char *name);
int							builtin_env(char **args);
int							builtin_exit(char **args);
int							remove_env_var(t_env **env, const char *name);
int							is_valid_identifier(const char *str);
int							process_export_arg(char *arg, t_env **env);
int							excute_commands(t_command *cmd);
int							is_builtin(char *cmd);
int							execute_builtin(char **args);

/* External commands */
char						*find_command(char *cmd);
int							execute_external_command(char **args);

/* Environment functions */
t_env						*create_env_list(char **envp);
void						free_env_list(t_env *env);
int							set_env_var(t_env **env, const char *name,
								const char *value);
int							remove_env_var(t_env **env, const char *name);

/* Environment array conversion */
char						**env_list_to_array(t_env *env);

/* Environment utility functions */
t_env	*create_env_node(const char *str);                      // プロトタイプを追加
t_env	*create_env_node2(const char *name, const char *value); // プロトタイプを追加
int							update_env_value(t_env *var, const char *value);
int							append_env_var(t_env **env, const char *name,
								const char *value);
t_env						*get_env_var(t_env *env, const char *name);
int							set_env_var(t_env **env, const char *name,
								const char *value);
int							validate_and_update_env(t_env **env,
								const char *name, const char *value);

/* Global environment variable */
t_env						**g_env(void);

/* Redirection functions */
t_redirect					*create_redirect(int type, char *file);
int							setup_redirection(t_redirect *redirect);
void						restore_redirection(t_redirect *redirect);
void						free_redirect(t_redirect *redirect);

/* Redirect utility functions */
int							save_original_fd(t_redirect *redirect);
int							open_redirect_file(t_redirect *redirect);
void						apply_redirection(t_redirect *redirect, int fd);
t_redir_type				map_token_to_redir_type(t_token_type token_redir_type);

/* External command utility functions */
char						*search_in_path(const char *path_env, char *cmd);

/* Redirect parsing functions */
t_redirect					*parse_redirect(char **args, int *cmd_end);

/* Command functions */
char						**prepare_command(char **args, int cmd_end);

/* printf */
size_t						ft_printf_fd(int fd, const char *format, ...);
size_t						ft_putchar_fd_len(char c, int fd);
size_t						ft_puthex_fd_len(unsigned long long n,
								int uppercase, int fd);
size_t						ft_putptr_fd_len(void *ptr, int fd);
size_t						ft_putull_fd_len(unsigned long long num, int fd);
size_t						ft_puthex_addr_fd_len(uintptr_t n, int fd);

t_env						**g_env(void);
t_env						*get_env_var(t_env *env, const char *name);

int							count_env_nodes(t_env *env);
char						*format_env_node_to_string(t_env *node);
void						free_array_upto_index(char **array, int index);

// parser_env_utils.c
t_env						*create_env_node2(const char *name,
								const char *value);

// env_list_to_array.c
char						**env_list_to_to_array(t_env *env);

// env_array_utils.c
void						free_env_array_entries(char **env_array, int count);
char						*allocate_env_entry(t_env *env_node);

// Forward declaration for t_command
typedef struct s_command	t_command;

// pipeline_execution.c
void						execute_pipeline_command(t_command *cmd,
								t_command *current);

/* パーサーの補助関数 */
bool						process_word_token(t_token **current_token_ptr,
								char **accumulated_arg_ptr);
bool						process_space_token(t_command *cmd_ptr,
								t_token **current_token_ptr,
								char **accumulated_arg_ptr);
bool						handle_error_token(t_token **current_token_ptr,
								char **accumulated_arg_ptr);
bool						handle_final_arg(t_command **cmd,
								char *accumulated_arg);
bool						add_accumulated_arg(t_command *cmd_ptr,
								char **accumulated_arg_ptr,
								const char *error_msg);
int							handle_redirect_token(t_token **curr_token,
								t_command *cmd);
t_token						*create_token(char *content, t_token_type type);
t_token						*add_token(t_token **head, const char *content,
								t_token_type type);
bool						parser_handle_pipe_token(t_command **cmd_ptr,
								t_token **current_token_ptr,
								t_command **head_cmd_ptr);
int							handle_pipe_token(t_token **current,
								t_command ***cmds, int *cmd_idx);
t_redir_type				convert_token_type_to_redir_type(t_token_type token_redir_type);

/* メモリ管理関数 */
void						free_accumulated_arg(char **accumulated_arg_ptr);
bool						handle_memory_error(void *ptr,
								char **accumulated_arg_ptr);
void						free_command_content(t_command *cmd);
bool						process_word_token(t_token **current_token_ptr,
								char **accumulated_arg_ptr);
bool						process_space_token(t_command *cmd_ptr,
								t_token **current_token_ptr,
								char **accumulated_arg_ptr);
bool						process_tokens(t_command **cmd, t_token *tokens,
								t_command **head);

#endif
