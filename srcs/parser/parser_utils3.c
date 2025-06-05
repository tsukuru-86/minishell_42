/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 05:45:59 by muiida            #+#    #+#             */
/*   Updated: 2025/06/05 04:01:04 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

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

/*
** 隣接する非空白トークンを結合して単一の文字列を作成
*/
char	*merge_adjacent_tokens(t_token **current_token)
{
	char	*result;
	t_token	*token;

	token = *current_token;
	if (!token)
		return (NULL);
	result = ft_strdup(token->content);
	if (!result)
		return (NULL);
	*current_token = token->next;
	return (result);
}
