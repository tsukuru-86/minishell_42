/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 04:10:30 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/16 22:45:57 by muiida           ###   ########.fr       */
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
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
/** # include <sys/syslimits.h> // macOS*/

# define MAX_TOKENS 1024

/* デバッグフラグ定数 */
# ifdef DEBUG
#  define DEBUG_ENABLED 1
# else
#  define DEBUG_ENABLED 0
# endif

/* エラーメッセージ定義 */
# define ERR_UNSET_NOT_ENOUGH_ARGS "minishell: unset: few arguments\n"
# define ERR_UNSET_INVALID_ID "minishell: unset: `%s': not a valid identifier\n"
# define ERR_EXPORT_MALLOC "minishell: export: memory allocation error\n"
# define ERR_EXPORT_INVALID_ID "minishell: export:`%s':not a valid identifier\n"
# define ERR_COMMAND_NOT_FOUND "minishell: %s: command not found\n"
# define ERR_SYNTAX_ERROR "minishell: syntax error\n"
# define ERR_UNEXP_TOKEN "minishell: syntax error near unexpected token `%s'\n"
# define ERR_UNCLOSED_QUOTE "minishell: syntax error: unclosed quote\n"
# define ERR_REDIRECTION_ERROR "minishell: redirection error\n"

extern volatile sig_atomic_t	g_signal;

void							print_env_array(const char *label, char **env);
int								is_delimiter(char c);
int								is_quote(char c);
int								is_meta(char c);

/* トークンの種類を定義 */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_SPACE,
	TOKEN_SINGLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
	TOKEN_S_QUOTED_WORD,
	TOKEN_D_QUOTED_WORD,
	TOKEN_EMPTY_QUOTED,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_ENV_VAR,
	TOKEN_NEWLINE,
	TOKEN_END
}								t_token_type;

void							setup_child_signals(void);

/* Redirection structure */
typedef struct s_redirect
{
	int							type;
	char						*file;
	int							original_fd;
	struct s_redirect			*next;
}								t_redirect;

/* パイプライン用の構造体 */
typedef struct s_pipeline
{
	int							read_fd;
	int							write_fd;
	pid_t						pid;
}								t_pipeline;

/* コマンド構造体 */
typedef struct s_command
{
	char						**args;
	t_redirect					*redirects;
	t_pipeline					pipe;
	struct s_command			*next;
	struct s_command			*prev;
	int							last_status;
}								t_command;

/* トークンを表す構造体 */
typedef struct s_token
{
	char						*content;
	t_token_type				type;
	struct s_token				*next;
}								t_token;

typedef struct s_tokenizer_stat
{
	t_token						*tokens;
	char						word_buffer[1024];
	int							i_input;
	t_command					*cmd;
	bool						needs_cmd_free;
	t_token_type				quote_type;
}								t_tokenizer_stat;

/* Environment variable structure */
typedef struct s_env
{
	char						*name;
	char						*value;
	struct s_env				*next;
}								t_env;

/* パーサー */
t_command						*parse_tokens(t_token *tokens);
void							free_command(t_command *cmd);
int								get_builtin_func_idx(char *cmd);

/* Redirection types */
typedef enum e_redir_type
{
	REDIR_OUT,
	REDIR_IN,
	REDIR_APPEND,
	REDIR_HEREDOC
}								t_redir_type;

/* Heredoc structure */
typedef struct s_heredoc
{
	char						*delimiter;
	char						*content;
	char						*temp_file;
	bool						is_closed;
	bool						delimiter_is_quoted;
}								t_heredoc;

/* Heredoc */
int								handle_heredoc(t_command *cmd, char *delimiter);
int								write_heredoc_content(int fd, char *content);
void							cleanup_heredoc(t_heredoc *heredoc);
t_heredoc						*init_heredoc(char *delimiter);

/* Command execution */
int								excute_commands(t_command *cmd);

/* Tokenizer */
t_token							*tokenize(char *input, t_command *cmd);
void							free_tokens(t_token *tokens);
void							print_tokens(t_token *tokens);

/* Debug functions */
void							print_command_debug(t_command *cmd);
void							print_commands_debug(t_command *commands);

/* Environment variable */
t_env							**get_env_val(void);
t_env							*create_env_list(char **envp);
void							free_env_list(void);
t_env							*create_env_node(const char *str);
char							*expand_env_vars(const char *str,
									int in_dquote);
t_env							*get_env_node(const char *name);
int								set_env_node(const char *name,
									const char *value);

/* Redirection - Public API */
void							restore_redirection(t_redirect *redirect);
int								setup_redirection(t_redirect *redirect);
void							free_redirect(t_redirect *redirect);
t_redirect						*create_redirect(int type, char *file);
void							cleanup_heredocs(t_redirect *redirect);
/* Command */
void							external_command(void);
int								execute_builtin(char **args);
int								execute_builtin_with_redirect(t_command *cmd);
int								execute_external_command(t_command *cmd);

/* ビルトインコマンド - Public API */
int								execute_builtin(char **args);
int								execute_builtin_with_redirect(t_command *cmd);
int								get_builtin_func_idx(char *cmd);

/* Pipeline */
int								execute_command_pipeline(t_command *cmd);
/* Exit status */
int								get_exit_status(void);
void							set_exit_status(t_command *cmd, int status);

/* Command handler */
void							handle_input(char *input, int *status);

/* Debug functions */
void							debug_print(const char *message, bool is_debug);
void							debug_print_with_str(const char *prefix,
									const char *str, bool is_debug);
void							debug_print_with_int(const char *prefix,
									int value, bool is_debug);
void							debug_print_tokens(t_token *tokens,
									bool is_debug);
void							debug_print_command_args(char **args,
									bool is_debug);

/* history_utils */
char							*get_history_path(void);
void							load_history_file(void);
void							save_history_file(void);

#endif
