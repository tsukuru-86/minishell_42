/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 08:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/13 16:19:13 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

/* パーサーモジュール内部ヘッダー
 * このヘッダーはparserディレクトリ内のファイルでのみ使用
 * 外部モジュールは minishell.h の関数のみを使用すること
 */

# include "minishell.h"

t_command	*create_command(void);
int			add_redirect(t_command *cmd, t_token *token, t_token *next_token);
int			add_argument(t_command *cmd, char *arg);
/* パーサー内部関数（外部からは呼び出し禁止） */
int			process_token_in_parse_loop(t_command **cmd_ptr,
				t_token **current_token_ptr, t_command **head_cmd_ptr);
void		link_commands(t_command *prev, t_command *next);
char		*merge_adjacent_tokens(t_token **current_token);
/* Token preprocessing functions */
int			expand_all_variables(t_token *tokens);
t_token		*remove_quote_tokens(t_token *tokens);
t_token		*merge_adjacent_non_meta_tokens(t_token *tokens);
t_token		*remove_space_tokens(t_token *tokens);
t_token		*preprocess_tokens(t_token *tokens);
t_token		*remove_empty_tokens(t_token *tokens);
void		cleanup_heredoc(t_heredoc *heredoc);
/* Heredoc input functions */
int			read_heredoc_input(int fd, t_heredoc *heredoc);
int			read_heredoc_tty(int fd, t_heredoc *heredoc);
int			read_heredoc_non_tty(int fd, t_heredoc *heredoc);
int			read_heredoc_from_pipe(int fd, t_heredoc *heredoc);
int			process_heredoc_line(char *line, int fd, t_heredoc *heredoc);
#endif
