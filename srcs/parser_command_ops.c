/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_ops.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 10:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/22 01:35:22 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
** 新しいコマンド構造体を作成
*/
t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
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

/*
** リダイレクトをコマンドに追加
*/
int	add_redirect(t_command *cmd, t_token *token, t_token *next_token)
{
	t_redirect	*redir;
	int			type;

	if (!next_token || (next_token->type != TOKEN_WORD
			&& next_token->type != TOKEN_SINGLE_QUOTE
			&& next_token->type != TOKEN_DOUBLE_QUOTE))
		return (0);
	if (token->type == TOKEN_REDIR_IN)
		type = REDIR_IN;
	else if (token->type == TOKEN_REDIR_OUT)
		type = REDIR_OUT;
	else if (token->type == TOKEN_REDIR_APPEND)
		type = REDIR_APPEND;
	else
		return (0);
	redir = create_redirect(type, next_token->content);
	if (!redir)
		return (0);
	redir->next = cmd->redirects;
	cmd->redirects = redir;
	return (1);
}

/*
** コマンド構造体の解放
*/
void	free_command(t_command *cmd)
{
	t_command	*next_cmd;
	int			i;

	while (cmd)
	{
		next_cmd = cmd->next;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
		}
		free_redirect(cmd->redirects);
		free(cmd);
		cmd = next_cmd;
	}
}
