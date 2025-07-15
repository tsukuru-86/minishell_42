/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/15 17:28:47 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_MESSAGES_H
# define ERROR_MESSAGES_H

/* Error messages for builtin commands */
# define ERR_UNSET_NOT_ENOUGH_ARGS "minishell: unset: few arguments\n"
# define ERR_UNSET_INVALID_ID "minishell: unset: `%s': not a valid identifier\n"
# define ERR_EXPORT_MALLOC "minishell: export: memory allocation error\n"
# define ERR_EXPORT_INVALID_ID "minishell: export: `%s': invalid identifier\n"

/* Error messages for command execution */
# define ERR_COMMAND_NOT_FOUND "minishell: %s: command not found\n"

/* Syntax error messages */
# define ERR_SYNTAX_ERROR "minishell: syntax error\n"
# define ERR_UNEXP_TOKEN "minishell: syntax error near unexpected token `%s'\n"
# define ERR_UNCLOSED_QUOTE "minishell: syntax error: unclosed quote\n"

/* Redirection error messages */
# define ERR_REDIRECTION_ERROR "minishell: redirection error\n"

#endif
