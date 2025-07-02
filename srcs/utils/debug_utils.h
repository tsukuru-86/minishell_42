/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:32:35 by muiida            #+#    #+#             */
/*   Updated: 2025/07/03 03:45:50 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_UTILS_H
# define DEBUG_UTILS_H

# include "minishell.h"

void		print_tokens_debug(const t_token *tokens, int debug);
const char	*get_token_type_str(t_token_type type);
void		debug_print(const char *message);
void		debug_print_token(const t_token *token);
void		debug_print_tokens(t_token *tokens);
void		debug_print_with_str(const char *prefix, const char *str);
void		debug_print_with_int(const char *prefix, int value);
void		print_args_debug(char **args);
void		debug_print_command_args(char **args);
void		print_commands_debug(t_command *commands);
#endif