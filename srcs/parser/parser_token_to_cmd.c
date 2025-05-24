/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_to_cmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 10:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/25 02:48:54 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/*
** @brief コマンド構造体を検証します。
** @param head コマンドリストの先頭
** @param tokens トークンリストの先頭
** @return 検証が成功した場合は1、失敗した場合は0
*/
static int	validate_command(t_command *head, t_token *tokens)
{
	if (head && head->args == NULL && head->redirects == NULL
		&& head->next == NULL && tokens->type == TOKEN_PIPE)
	{
		return (0);
	}
	return (1);
}

/*
** @brief トークン列をコマンド構造体に変換するメインループ処理です。
** @param tokens トークンリストの先頭
** @return 変換されたコマンドリストの先頭、またはエラー時にNULL
*/
static t_command	*parse_tokens_loop(t_token *tokens)
{
	t_command	*cmd;
	t_command	*head;
	t_token		*current;

	cmd = create_command();
	if (!cmd)
	{
		ft_putstr_fd("minishell: failed to create command structure\n", 2);
		return (NULL);
	}
	head = cmd;
	current = tokens;
	while (current)
	{
		if (process_token_in_parse_loop(&cmd, &current, &head) == 0)
			return (NULL);
	}
	return (head);
}

/*
** @brief トークン列をコマンド構造体に変換します。
** @param tokens トークンリストの先頭
** @return 変換されたコマンドリストの先頭、またはエラー時にNULL
*/
t_command	*parse_tokens(t_token *tokens)
{
	t_command	*head;

	if (!tokens)
		return (NULL);
	head = parse_tokens_loop(tokens);
	if (!head || !validate_command(head, tokens))
		return (NULL);
	return (head);
}
