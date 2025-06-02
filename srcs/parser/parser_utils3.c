/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 05:45:59 by muiida            #+#    #+#             */
/*   Updated: 2025/06/03 05:32:25 by muiida           ###   ########.fr       */
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
	char	*temp;
	t_token	*token;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	token = *current_token;
	while (token && token->type != TOKEN_SPACE && token->type != TOKEN_PIPE
		&& token->type != TOKEN_REDIR_IN && token->type != TOKEN_REDIR_OUT
		&& token->type != TOKEN_REDIR_APPEND && token->type != TOKEN_HEREDOC)
	{
		temp = ft_strjoin(result, token->content);
		free(result);
		if (!temp)
			return (NULL);
		result = temp;
		token = token->next;
	}
	*current_token = token;
	return (result);
}
