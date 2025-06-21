/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:17:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/21 22:23:57 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_UTILS_H
# define INPUT_UTILS_H

# include "minishell.h"

char	*prepare_input(char *input);
char	*read_all_pipe_input(void);
int		main_loop(void);
int		handle_empty_input(char *input);
void	process_valid_input(char *input, int *status);
int		handle_tokens_and_parse(t_token *tokens);
void	free_string_array(char **arr);
void	process_input_lines(char **lines, int *status);
void	process_with_fallback(char *input, int *status);
#endif