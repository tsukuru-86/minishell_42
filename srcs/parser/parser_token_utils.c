/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 21:32:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/16 21:32:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/error_messages.h"
#include "minishell.h"
#include "parser.h"

static int	handle_heredoc_redirect(t_command *cmd, t_token **current_token,
		t_command **head_cmd)
{
	t_token	*delimiter_token;

	delimiter_token = (*current_token)->next;
	if (!add_redirect(cmd, *current_token, delimiter_token))
	{
		handle_heredoc_error(head_cmd);
		return (0);
	}
	if (!handle_heredoc(cmd, delimiter_token->content))
	{
		handle_heredoc_error(head_cmd);
		return (0);
	}
	skip_to_delimiter(current_token, delimiter_token->content);
	return (1);
}

/*
** @brief WORDタイプのトークンを処理します。
** @param cmd_ptr 現在のコマンド構造体へのポインタ
** @param current_token_ptr 現在のトークンへのポインタ
** @param head_cmd_ptr コマンドリストの先頭へのポインタ
** @return 成功した場合は1、失敗した場合は0
*/
int	handle_word_type_tokens(t_command **cmd_ptr, t_token **current_token_ptr,
		t_command **head_cmd_ptr)
{
	int	status;

	debug_print("[DEBUG] Handling WORD token", DEBUG_ENABLED);
	status = handle_word_token(*cmd_ptr, current_token_ptr, head_cmd_ptr);
	return (status);
}

/*
** @brief リダイレクトトークンを処理します。
** @param cmd 現在のコマンド構造体
** @param current_token 現在のトークンへのポインタ
** @param head_cmd コマンドリストの先頭へのポインタ
** @return 成功した場合は1、失敗した場合は0
*/
static int	handle_normal_redirect(t_command *cmd, t_token **current_token,
		t_command **head_cmd)
{
	if (!add_redirect(cmd, *current_token, (*current_token)->next))
	{
		if (*head_cmd)
		{
			free_command(*head_cmd);
		}
		ft_putstr_fd((char *)ERR_REDIRECTION_ERROR, 2);
		return (0);
	}
	*current_token = (*current_token)->next;
	if (*current_token)
		*current_token = (*current_token)->next;
	return (1);
}

static int	handle_empty_cmd_redirect(t_command *cmd, t_token **current_token,
		t_command **head_cmd)
{
	debug_print("[DEBUG] Empty command with redirect", DEBUG_ENABLED);
	if (!add_redirect(cmd, *current_token, (*current_token)->next))
	{
		if (*head_cmd)
			free_command(*head_cmd);
		ft_putstr_fd((char *)ERR_REDIRECTION_ERROR, 2);
		return (0);
	}
	*current_token = (*current_token)->next;
	if (*current_token)
		*current_token = (*current_token)->next;
	return (1);
}

int	handle_redirect_token(t_command *cmd, t_token **current_token,
		t_command **head_cmd)
{
	if (!cmd || !current_token || !*current_token || !head_cmd)
		return (0);
	if (!cmd->args || !cmd->args[0])
		return (handle_empty_cmd_redirect(cmd, current_token, head_cmd));
	if (!((*current_token)->next))
	{
		if (*head_cmd)
			free_command(*head_cmd);
		ft_printf_fd(2, ERR_UNEXP_TOKEN, "newline");
		*head_cmd = NULL;
		return (0);
	}
	if ((*current_token)->type == TOKEN_HEREDOC)
		return (handle_heredoc_redirect(cmd, current_token, head_cmd));
	return (handle_normal_redirect(cmd, current_token, head_cmd));
}
