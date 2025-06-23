/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_handlers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 10:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/17 10:00:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/error_messages.h"
#include "minishell.h"
#include "parser.h"

/*
** @brief REDIRECTタイプのトークンを処理します。
** @param cmd_ptr 現在のコマンド構造体へのポインタ
** @param current_token_ptr 現在のトークンへのポインタ
** @param head_cmd_ptr コマンドリストの先頭へのポインタ
** @return 成功した場合は1、失敗した場合は0
*/
int	handle_redirect_type_tokens(t_command **cmd_ptr,
		t_token **current_token_ptr, t_command **head_cmd_ptr)
{
	int	status;

	debug_print("[DEBUG] Handling REDIRECT token", DEBUG_ENABLED);
	status = handle_redirect_token(*cmd_ptr, current_token_ptr, head_cmd_ptr);
	return (status);
}

/*
** @brief その他のトークンを処理します。
** @param current_token_ptr 現在のトークンへのポインタ
** @param type トークンのタイプ
** @return 常に1
*/
int	handle_other_tokens(t_token **current_token_ptr, t_token_type type)
{
	if (type == TOKEN_SPACE)
	{
		debug_print("[DEBUG] Skipping SPACE token", DEBUG_ENABLED);
	}
	else if (type == TOKEN_NEWLINE)
	{
		debug_print("[DEBUG] Skipping NEWLINE token", DEBUG_ENABLED);
	}
	else
	{
		debug_print("[DEBUG] Skipping OTHER token", DEBUG_ENABLED);
	}
	*current_token_ptr = (*current_token_ptr)->next;
	return (1);
}
