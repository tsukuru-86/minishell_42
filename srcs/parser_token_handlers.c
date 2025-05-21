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

#include "../minishell.h"

/*
** @brief 単語トークンを処理します。
** @param cmd 現在のコマンド構造体
** @param current_token 現在のトークンへのポインタ
** @param head_cmd コマンドリストの先頭へのポインタ
** @return 成功した場合は1、失敗した場合は0
*/
int	handle_word_token(t_command *cmd, t_token **current_token,
		t_command **head_cmd)
{
	if (!add_argument(cmd, (*current_token)->content))
	{
		free_command(*head_cmd);
		return (0);
	}
	*current_token = (*current_token)->next;
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
	if (!(*current_token)->next)
	{
		free_command(*head_cmd);
		ft_putstr_fd("minishell: syntax error ", 2);
		ft_putstr_fd("near unexpected token `newline'\n", 2);
		return (0);
	}
	if (!add_redirect(cmd, *current_token, (*current_token)->next))
	{
		free_command(*head_cmd);
		ft_putstr_fd("minishell: redirection error\n", 2);
		return (0);
	}
	*current_token = (*current_token)->next->next;
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
	(*cmd)->next = create_command();
	if (!(*cmd)->next)
	{
		free_command(*head_cmd);
		*head_cmd = NULL;
		return (0);
	}
	*cmd = (*cmd)->next;
	*current_token = (*current_token)->next;
	if (!*current_token)
	{
		ft_putstr_fd("minishell: syntax error ", 2);
		ft_putstr_fd("near unexpected token `|'\n", 2);
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

	status = 1;
	type = (*current_token_ptr)->type;
	if (type == TOKEN_WORD || type == TOKEN_SINGLE_QUOTE
		|| type == TOKEN_DOUBLE_QUOTE)
		status = handle_word_token(*cmd_ptr, current_token_ptr, head_cmd_ptr);
	else if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND)
		status = handle_redirect_token(*cmd_ptr, current_token_ptr,
				head_cmd_ptr);
	else if (type == TOKEN_PIPE)
		status = handle_pipe_token(cmd_ptr, current_token_ptr, head_cmd_ptr);
	else
		*current_token_ptr = (*current_token_ptr)->next;
	return (status);
}
