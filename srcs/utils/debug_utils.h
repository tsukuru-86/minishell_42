/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:32:35 by muiida            #+#    #+#             */
/*   Updated: 2025/06/12 17:35:31 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_UTILS_H
# define DEBUG_UTILS_H

# include "minishell.h"

void	print_tokens_debug(const t_token *tokens, int debug);
void	print_args_debug(char **args, int debug);

int	handle_tokens_and_parse(t_token *tokens, int debug);

#endif