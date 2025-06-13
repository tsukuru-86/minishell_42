/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_handlers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 10:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/22 10:00:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/error_messages.h"
#include "minishell.h"
#include "parser.h"

/*
** @brief 単語トークンを処理します。隣接する非空白トークンを結合します。
** @param cmd 現在のコマンド構造体
** @param current_token 現在のトークンへのポインタ
** @param head_cmd コマンドリストの先頭へのポインタ
** @return 成功した場合は1、失敗した場合は0
*/
int	handle_word_token(t_command *cmd, t_token **current_token,
		t_command **head_cmd)
{
	char	*merged_content;

	if (!cmd || !current_token || !*current_token || !head_cmd)
		return (0);
	merged_content = merge_adjacent_tokens(current_token);
	if (!merged_content)
	{
		if (*head_cmd)
			free_command(*head_cmd);
		return (0);
	}
	if (!add_argument(cmd, merged_content))
	{
		free(merged_content);
		if (*head_cmd)
			free_command(*head_cmd);
		return (0);
	}
	free(merged_content);
	return (1);
}

/*
** @brief リダイレクトトークンを処理します。
** @param cmd 現在のコマンド構造体
** @param current_token 現在のトークンへのポインタ
** @param head_cmd コマンドリストの先頭へのポインタ
** @return 成功した場合は1、失敗した場合は0
*/
static int	handle_redirect_token(t_command *cmd, t_token **current_token,
		t_command **head_cmd)
{
	if (!cmd || !current_token || !*current_token || !head_cmd)
		return (0);
	if (!((*current_token)->next))
	{
		if (*head_cmd)
			free_command(*head_cmd);
		ft_printf_fd(2, ERR_UNEXP_TOKEN, "newline");
		*head_cmd = NULL;
		return (0);
	}
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

/*
** @brief パイプトークンを処理します。
** @param cmd 現在のコマンド構造体へのポインタ
** @param current_token 現在のトークンへのポインタ
** @param head_cmd コマンドリストの先頭へのポインタ
** @return 成功した場合は1、失敗した場合は0
*/
int	handle_pipe_token(t_command **cmd, t_token **current_token,
		t_command **head_cmd)
{
	if (!cmd || !*cmd || !current_token || !*current_token || !head_cmd)
		return (0);
	(*cmd)->next = create_command();
	if (!(*cmd)->next)
	{
		if (*head_cmd)
			free_command(*head_cmd);
		*head_cmd = NULL;
		return (0);
	}
	*cmd = (*cmd)->next;
	*current_token = (*current_token)->next;
	if (!*current_token)
	{
		ft_printf_fd(2, ERR_UNEXP_TOKEN, "|");
		if (*head_cmd)
			free_command(*head_cmd);
		*head_cmd = NULL;
		return (0);
	}
	return (1);
}

/*
** @brief parse_tokensのループ内でのトークン処理を行います。
** @param cmd_ptr 現在のコマンド構造体へのポインタ
** @param current_token_ptr 現在のトークンへのポインタ
** @param head_cmd_ptr コマンドリストの先頭へのポインタ
** @return 成功した場合は1、失敗した場合は0
*/
int	process_token_in_parse_loop(t_command **cmd_ptr,
		t_token **current_token_ptr, t_command **head_cmd_ptr)
{
	t_token_type	type;
	int				status;

	if (!cmd_ptr || !*cmd_ptr || !current_token_ptr || !*current_token_ptr
		|| !head_cmd_ptr)
		return (0);
	status = 1;
	type = (*current_token_ptr)->type;
	if (type == TOKEN_WORD || type == TOKEN_S_QUOTED_WORD
		|| type == TOKEN_D_QUOTED_WORD)
		status = handle_word_token(*cmd_ptr, current_token_ptr, head_cmd_ptr);
	else if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC)
	{
		status = handle_redirect_token(*cmd_ptr, current_token_ptr,
				head_cmd_ptr);
	}
	else if (type == TOKEN_PIPE)
		status = handle_pipe_token(cmd_ptr, current_token_ptr, head_cmd_ptr);
	else if (type == TOKEN_SPACE)
		*current_token_ptr = (*current_token_ptr)->next;
	else
		*current_token_ptr = (*current_token_ptr)->next;
	return (status);
}
