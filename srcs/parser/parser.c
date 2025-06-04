/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 10:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/05 00:15:41 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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

/*
** 引数配列のサイズを拡張し、既存の引数をコピー
*/
static char	**resize_and_copy_args(char **old_args, int current_arg_count)
{
	char	**new_args;
	int		i;

	new_args = (char **)malloc(sizeof(char *) * (current_arg_count + 2));
	if (!new_args)
		return (NULL);
	i = 0;
	if (old_args)
	{
		while (i < current_arg_count)
		{
			new_args[i] = old_args[i];
			i++;
		}
	}
	return (new_args);
}

/*
** トークンをコマンド引数に追加
*/
int	add_argument(t_command *cmd, char *arg)
{
	int		i;
	char	**new_args;

	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
			i++;
	}
	new_args = resize_and_copy_args(cmd->args, i);
	if (!new_args)
		return (0);
	new_args[i] = ft_strdup(arg);
	if (!new_args[i])
	{
		free(new_args);
		return (0);
	}
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	return (1);
}

/* トークンタイプに基づいてリダイレクトタイプを決定し、ヒアドキュメントを処理 */
static int	determine_redirect_type(t_command *cmd, t_token *token,
		t_token *next_token, int *type_out)
{
	if (token->type == TOKEN_REDIR_IN)
		*type_out = REDIR_IN;
	else if (token->type == TOKEN_REDIR_OUT)
		*type_out = REDIR_OUT;
	else if (token->type == TOKEN_REDIR_APPEND)
		*type_out = REDIR_APPEND;
	else if (token->type == TOKEN_HEREDOC)
	{
		if (!handle_heredoc(cmd, next_token->content))
			return (0);
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

	if (!next || (next->type != TOKEN_WORD && next->type != TOKEN_SINGLE_QUOTE
			&& next->type != TOKEN_DOUBLE_QUOTE))
		return (0);
	result = determine_redirect_type(cmd, token, next, &type);
	if (result == 0)
		return (0);
	if (result == 2)
		return (1);
	redir = create_redirect(type, next->content);
	if (!redir)
		return (0);
	if (!cmd->redirects)
		cmd->redirects = redir;
	else
	{
		current = cmd->redirects;
		while (current->next)
			current = current->next;
		current->next = redir;
	}
	return (1);
}
