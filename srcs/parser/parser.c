/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 10:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/09 02:21:14 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
** 新しいコマンド構造体を作成
*/
t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
	{
		ft_putstr_fd("minishell: failed to create command structure\n", 2);
		return (NULL);
	}
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}

/* トークンタイプに基づいてリダイレクトタイプを決定し、ヒアドキュメントを処理 */
static int	determine_redirect_type(t_command *cmd, t_token *token,
		t_token *next_token, int *type_out)
{
	if (!cmd || !token || !next_token || !type_out)
		return (0);
	if (token->type == TOKEN_REDIR_IN)
		*type_out = REDIR_IN;
	else if (token->type == TOKEN_REDIR_OUT)
		*type_out = REDIR_OUT;
	else if (token->type == TOKEN_REDIR_APPEND)
		*type_out = REDIR_APPEND;
	else if (token->type == TOKEN_HEREDOC)
	{
		if (!handle_heredoc(cmd, next_token))
		{
			return (0);
		}
		return (2);
	}
	else
		return (0);
	return (1);
}

/* リダイレクトをコマンドに追加 */
int	add_redirect(t_command *cmd, t_token *token, t_token *next)
{
	t_redirect	*redir;
	t_redirect	*current;
	int			type;
	int			result;

	if (!cmd || !token || !next || (next->type != TOKEN_WORD
			&& next->type != TOKEN_SINGLE_QUOTE
			&& next->type != TOKEN_DOUBLE_QUOTE))
		return (0);
	result = determine_redirect_type(cmd, token, next, &type);
	if (result != 1)
		return (result == 2);
	redir = create_redirect(type, next->content);
	if (!redir)
		return (0);
	if (!cmd->redirects)
	{
		cmd->redirects = redir;
		return (1);
	}
	current = cmd->redirects;
	while (current && current->next)
		current = current->next;
	current->next = redir;
	return (1);
}
