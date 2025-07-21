/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:32:35 by muiida            #+#    #+#             */
/*   Updated: 2025/07/16 05:34:38 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# include "minishell.h"

int		dbg_printf(const char *format, ...);
void	debug_print_tokens_list(t_token *tokens);
void	debug_print_command_args(char **args);
#endif