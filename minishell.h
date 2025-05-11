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
	TOKEN_END           // 終端
}					t_token_type;

/* トークンを表す構造体 */
typedef struct s_token
{
	char			*content;
	t_token_type	type;
	struct s_token	*next;
}					t_token;

t_token				*tokenize(char *input);
void				free_tokens(t_token *tokens);
void				print_tokens(t_token *tokens);

/* Environment variable structure */
typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}					t_env;

/* Environment functions */
t_env				*create_env_list(char **envp);
void				free_env_list(t_env *env);
t_env				*get_env_var(t_env *env, const char *name);
int					set_env_var(t_env **env, const char *name,
						const char *value);
int					remove_env_var(t_env **env, const char *name);

/* Redirection types */
# define REDIR_OUT 1 /* > */
# define REDIR_IN 2 /* < */
# define REDIR_APPEND 3 /* >> */

/* Redirection structure */
typedef struct s_redirect
{
	int				type;
	char			*file;
	int				original_fd;
}					t_redirect;

void				free_tokens(char **tokens);
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
int					excute_commands(t_token *tokens, char **envp);

/* External commands */
char				*find_command(char *cmd, char **envp);
int					execute_external_command(char **args, char **envp);

/* Global environment variable */
extern t_env		*g_env;

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
char				*get_path_env(t_env *env);
char				*search_in_path(const char *path_env, char *cmd);

/* Redirect parsing functions */
t_redirect			*parse_redirect(char **args, int *cmd_end);
/* Command preparation functions */
char				**prepare_command(char **args, int cmd_end);

#endif
