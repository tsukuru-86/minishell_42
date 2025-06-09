/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 05:45:59 by muiida            #+#    #+#             */
/*   Updated: 2025/06/09 16:47:20 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/*
** コマンド引数配列の解放
*/
static void	free_command_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
}

/*
** コマンド構造体の解放
*/
void	free_command(t_command *cmd)
{
	t_command	*next_cmd;

	while (cmd)
	{
		next_cmd = cmd->next;
		if (cmd->args)
			free_command_args(cmd->args);
		if (cmd->redirects)
			free_redirect(cmd->redirects);
		free(cmd);
		cmd = next_cmd;
	}
}

/*
** 隣接する非空白トークンを結合して単一の文字列を作成
*/
char	*merge_adjacent_tokens(t_token **current_token)
{
	char	*result;
	t_token	*token;

	if (!current_token || !*current_token)
		return (NULL);
	token = *current_token;
	if (!token->content)
		return (NULL);
	result = ft_strdup(token->content);
	if (!result)
		return (NULL);
	*current_token = token->next;
	return (result);
}
