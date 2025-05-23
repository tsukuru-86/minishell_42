/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 08:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/24 05:27:08 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

/* パーサーモジュール内部ヘッダー
 * このヘッダーはparserディレクトリ内のファイルでのみ使用
 * 外部モジュールは minishell.h の関数のみを使用すること
 */

# include "../../minishell.h"

/* パーサー内部関数（外部からは呼び出し禁止） */
int		add_argument(t_command *cmd, char *arg);
int		determine_redirect_type(t_command *cmd, t_token *token, 
			t_token *next_token, int *type_out);
int		process_token_in_parse_loop(t_command **cmd_ptr,
			t_token **current_token_ptr, t_command **head_cmd_ptr);

/* パーサーユーティリティ（モジュール内部のみ） */
t_command	*create_command(void);
void		link_commands(t_command *prev, t_command *next);

#endif
