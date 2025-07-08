/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_to_cmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 10:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/09 02:41:52 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "tokenizer/tokenizer.h"
#include "error/error_messages.h"

/*
** @brief 先頭のトークンが構文エラーでないかをチェック
*/
static int	check_trailing_pipe(t_token *tokens)
{
	t_token	*current;
	t_token	*last_non_space;

	if (!tokens)
		return (1);
	last_non_space = NULL;
	current = tokens;
	while (current)
	{
		if (current->type != TOKEN_SPACE && current->type != TOKEN_NEWLINE)
			last_non_space = current;
		current = current->next;
	}
	if (last_non_space && last_non_space->type == TOKEN_PIPE)
	{
		ft_printf_fd(2, ERR_UNEXP_TOKEN, "|");
		return (0);
	}
	return (1);
}

static int	check_syntax_errors(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (!check_trailing_pipe(tokens))
		return (0);
	return (1);
}

/*
** @brief コマンド構造体を検証します。
** @param head コマンドリストの先頭
** @param tokens トークンリストの先頭
** @return 検証が成功した場合は1、失敗した場合は0
*/
static int	validate_command(t_command *head, t_token *tokens)
{
	if (!check_syntax_errors(tokens))
		return (0);
	if (!check_advanced_syntax(tokens))
		return (0);
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
	t_token		*preprocessed_tokens;
	t_token		*first_token;

	if (!tokens)
		return (NULL);
	preprocessed_tokens = preprocess_tokens(tokens);
	if (!preprocessed_tokens)
	{
		return (create_command());
	}
	first_token = preprocessed_tokens;
	while (first_token && first_token->type == TOKEN_SPACE)
		first_token = first_token->next;
	head = parse_tokens_loop(preprocessed_tokens);
	if (!head || !validate_command(head, preprocessed_tokens))
	{
		free_tokens(preprocessed_tokens);
		return (NULL);
	}
	free_tokens(preprocessed_tokens);
	return (head);
}
