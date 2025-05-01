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

#endif