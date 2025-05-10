/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 04:10:30 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/11 00:27:38 by muiida           ###   ########.fr       */
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

/* Redirection types */
# define REDIR_OUT 1    /* > */
# define REDIR_IN 2     /* < */
# define REDIR_APPEND 3 /* >> */

/* Redirection structure */
typedef struct s_redirect
{
	int				type;
	char			*file;
	int				original_fd;
}					t_redirect;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}					t_env;

void				free_tokens(char **tokens);
char				**split_command(char *input);
int					excute_commands(char **args, char **envp);

/* Built-in commands */
int					builtin_echo(char **args);
int					builtin_cd(char **args);
int					builtin_pwd(char **args);
int					builtin_export(char **args);
int					builtin_unset(char **args);
int					builtin_env(char **args);
int					builtin_exit(char **args);
int					remove_env_var(t_env **env, const char *name);
int					set_env_var(t_env **env, const char *name,
						const char *value);

/* External commands */
char				*find_command(char *cmd, char **envp);
int					execute_external_command(char **args, char **envp);

/* Redirection functions */
t_redirect			*create_redirect(int type, char *file);
int					setup_redirection(t_redirect *redirect);
void				restore_redirection(t_redirect *redirect);
void				free_redirect(t_redirect *redirect);

#endif
