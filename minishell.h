/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 04:10:30 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/18 03:34:10 by muiida           ###   ########.fr       */
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
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}						t_redir_type;

/* トークンの種類を定義 */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_SPACE,
	TOKEN_SINGLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
	TOKEN_QUOTED_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_ENV_VAR,
	TOKEN_HEREDOC,
	TOKEN_END
}						t_token_type;

/* トークンを表す構造体 */
typedef struct s_token
{
	char				*content;
	t_token_type		type;
	struct s_token		*next;
}						t_token;

/* Redirection structure */
typedef struct s_redirect
{
	t_redir_type		type;
	char				*file;
	int					original_fd;
	struct s_redirect	*next;
}						t_redirect;

/* パイプライン用の構造体 */
typedef struct s_pipeline
{
	int					read_fd;
	int					write_fd;
	pid_t				pid;
}						t_pipeline;

/* コマンド構造体 */
typedef struct s_command
{
	char				**args;
	t_redirect			*redirects;
	t_pipeline			pipe;
	struct s_command	*next;
	struct s_command	*prev;
}						t_command;

/* Environment variable structure */
typedef struct s_env
{
	char				*name;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_minishell
{
	t_env				*env;
	char				**argv;
	int					argc;
	char				*line;
	int					exit_status;
}						t_minishell;

/* 環境変数展開用の構造体 */
typedef struct s_expand
{
	const char			*str;
	char				*res;
	int					i;
	int					j;
	bool				in_dquote;
	t_minishell			*shell;
}						t_expand;

/* global value holder */
t_minishell				**shell_state_holder(void);

/* パーサー関数 */
t_command				*parse_tokens(t_token *tokens);
void					free_command(t_command *cmd);
t_command				*create_command(void);
int						add_argument(t_command *cmd, char *arg);
t_command				*initialize_command_list(void);
bool					process_tokens(t_command **cmd, t_token *tokens);
bool					process_token_in_parse_loop(t_command **cmd_ptr,
							t_token **current_token_ptr,
							char **accumulated_arg_ptr);

/* パーサー補助関数 */
bool					process_word_token(t_token **current_token_ptr,
							char **accumulated_arg_ptr);
bool					process_space_token(t_command *cmd_ptr,
							t_token **current_token_ptr,
							char **accumulated_arg_ptr);
bool					handle_error_token(t_token **current_token_ptr,
							char **accumulated_arg_ptr);
bool					handle_final_arg(t_command **cmd,
							char *accumulated_arg);
bool					add_accumulated_arg(t_command *cmd_ptr,
							char **accumulated_arg_ptr, const char *error_msg);
int						handle_redirect_token(t_token **curr_token,
							t_command *cmd);
t_token					*add_token(t_token **head, const char *content,
							t_token_type type);
int						handle_pipe_token(t_token **current, t_command ***cmds,
							int *cmd_idx);
bool					handle_memory_error(void *ptr,
							char **accumulated_arg_ptr);

/* Tokenizer functions */
t_token					*tokenize(char *input, t_minishell *shell);
void					free_tokens(t_token *tokens);
void					print_tokens(t_token *tokens);
int						is_delimiter(char c);
int						is_quote(char c);
int						is_meta(char c);
t_token					*create_meta_token(char *input, int *i);
t_token					*create_token(char *content, t_token_type type);
int						handle_word(char *input, int *i, t_token **tokens,
							char *word_buf, t_minishell *shell);
int						handle_meta_character(char *input, int *i,
							t_token **tokens);
int						handle_quoted_string(char *input, int *i,
							t_token **tokens, char *word_buf,
							t_minishell *shell);
void					launch_child(char *cmd_path, char **args);
int						execute_command(t_command *cmd, t_minishell *shell);

/* Built-in commands */
int						builtin_echo(char **args);
int						builtin_cd(char **args);
int						builtin_pwd(char **args);
int						builtin_export(char **args);
int						builtin_unset(char **args);
int						builtin_env(char **args);
int						builtin_exit(char **args);
int						is_builtin(char *cmd);
int						execute_builtin(t_command *cmd);
int						check_identifier_for_unset(const char *name);
int						validate_identifier_for_removal(const char *name);
int						remove_env_var(t_env **env, const char *name);
int						is_valid_identifier(const char *str);
int						process_export_arg(char *arg, t_env **env);

/* Environment functions */
t_env					**g_env(void);
t_env					*get_env_var(t_env *env, const char *name);
t_env					*create_env_list(char **envp);
void					free_env_list(t_env *env);
int						set_env_var(t_env **env, const char *name,
							const char *value);
t_env					*create_env_node(const char *str);
t_env					*create_env_node2(const char *name, const char *value);
int						update_env_value(t_env *var, const char *value);
int						validate_and_update_env(t_env **env, const char *name,
							const char *value);

/* Signal handling functions */
void					init_signal_handlers(void);
void					setup_child_signals(void);
void					setup_parent_signals(void);
void					restore_default_signals(void);
void					handle_signals(void);
int						get_signal_num(void);
void					set_signal_num(int signum);

/* Environment variable expansion */
char					*expand_env_var(const char *name, t_minishell *shell);
char					*expand_special_var(const char *name,
							t_minishell *shell);
void					handle_quote(t_expand *exp);
int						append_env(t_expand *exp);
int						append_env_var(t_env **env_list, const char *name,
							const char *value);
char					*expand_env_vars(const char *str, bool in_dquote,
							t_minishell *shell);
char					*cleanup_expand(t_expand *exp, char *result);
char					*extract_env_name(const char *str);

/* Environment array conversion */
void					free_single_env_node(t_env *node);
char					**env_list_to_array(t_env *env);
void					free_env_array_entries(char **env_array, int count);
char					*allocate_env_entry(t_env *env_node);
int						count_env_nodes(t_env *env);
char					*format_env_node_to_string(t_env *node);
void					free_array_upto_index(char **array, int index);

/* Pipeline functions */
int						setup_pipeline(t_command *cmd);
void					cleanup_pipeline(t_command *cmd);
int						wait_pipeline(t_command *cmd);
void					pipeline_close_pipes(t_command *cmd);
void					execute_command_in_pipeline(t_command *cmd,
							t_command *current);
int						execute_pipeline_command(t_command *cmd,
							t_minishell *shell);
int						execute_single_command(t_command *cmd,
							t_minishell *shell);
;
/* Redirection functions */
t_redirect				*create_redirect(int type, char *file);
int						setup_redirection(t_redirect *redirect);
void					restore_redirection(t_redirect *redirect);
void					free_redirect(t_redirect *redirect);
int						save_original_fd(t_redirect *redirect);
int						open_redirect_file(t_redirect *redirect);
void					apply_redirection(t_redirect *redirect, int fd);
t_redir_type			map_token_to_redir_type(t_token_type token_type);

/* External command functions */
char					*find_command(char *cmd);
int						execute_external_command(t_command *cmd);
char					*search_in_path(const char *path_env, char *cmd);

/* Command functions */
char					**split_command(char *input);
t_redirect				*parse_redirect(char **args, int *cmd_end);
char					**prepare_command(char **args, int cmd_end);

/* printf */
size_t					ft_printf_fd(int fd, const char *format, ...);
size_t					ft_putchar_fd_len(char c, int fd);
size_t					ft_puthex_fd_len(unsigned long long n, int uppercase,
							int fd);
size_t					ft_putptr_fd_len(void *ptr, int fd);
size_t					ft_putull_fd_len(unsigned long long num, int fd);
size_t					ft_puthex_addr_fd_len(uintptr_t n, int fd);

#endif
