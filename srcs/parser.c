/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:13:41 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/19 00:13:04 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 新しいコマンド構造体を作成 */
static t_command	*create_command(void)
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

/* 引数配列のサイズを拡張し、既存の引数をコピー */
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

/* トークンをコマンド引数に追加 */
static int	add_argument(t_command *cmd, char *arg)
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
		free(new_args); // resize_and_copy_argsで確保したメモリを解放
		return (0);
	}
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	return (1);
}

/* リダイレクトをコマンドに追加 */
static int	add_redirect(t_command *cmd, t_token *token, t_token *next)
{
	t_redirect	*redir;
	int			type;

	// 修正: リダイレクト先もクォートされうる
	if (!next || (next->type != TOKEN_WORD && next->type != TOKEN_SINGLE_QUOTE
			&& next->type != TOKEN_DOUBLE_QUOTE))
		return (0); // エラー: リダイレクト先がないか、不正なトークンタイプ
	// リダイレクトタイプの判定
	if (token->type == TOKEN_REDIR_IN)
		type = REDIR_IN;
	else if (token->type == TOKEN_REDIR_OUT)
		type = REDIR_OUT;
	else if (token->type == TOKEN_REDIR_APPEND)
		type = REDIR_APPEND;
	else
		return (0); // 不明なリダイレクトタイプ (通常は到達しない)
	// リダイレクト構造体の作成
	redir = create_redirect(type, next->content);
	if (!redir)
		return (0); // メモリ確保失敗
	// リダイレクトリストの先頭に追加
	redir->next = cmd->redirects;
	cmd->redirects = redir;
	return (1);
}

/* コマンド構造体の解放 */
void	free_command(t_command *cmd)
{
	t_command	*next_cmd;
	int			i;

	while (cmd)
	{
		next_cmd = cmd->next;
		// 引数配列の解放
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
		}
		// リダイレクトの解放
		free_redirect(cmd->redirects);
		free(cmd);
		cmd = next_cmd;
	}
}

/* 単語トークンを処理 */
static int	handle_word_token(t_command *cmd, t_token **current_token,
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

/* リダイレクトトークンを処理 */
static int	handle_redirect_token(t_command *cmd, t_token **current_token,
		t_command **head_cmd)
{
	if (!(*current_token)->next) // リダイレクト先のトークンが存在しない
	{
		free_command(*head_cmd);
		ft_putstr_fd((char *)"minishell: syntax error near unexpected token `newline'\n", 2);
		return (0);
	}
	if (!add_redirect(cmd, *current_token, (*current_token)->next))
	{
		free_command(*head_cmd);
		ft_putstr_fd((char *)"minishell: redirection error\n", 2);
		return (0);
	}
	*current_token = (*current_token)->next->next;
	return (1);
}

/* パイプトークンを処理 */
static int	handle_pipe_token(t_command **cmd, t_token **current_token,
		t_command **head_cmd)
{
	(*cmd)->next = create_command();
	if (!(*cmd)->next)
	{
		free_command(*head_cmd);
		*head_cmd = NULL; // head_cmdをNULLにして解放済みを示す
		return (0);
	}
	*cmd = (*cmd)->next;
	*current_token = (*current_token)->next;
	if (!*current_token) // パイプの後にコマンドがない
	{
		ft_putstr_fd((char *)"minishell: syntax error near unexpected token `|'\n", 2);
		free_command(*head_cmd);
		*head_cmd = NULL; // head_cmdをNULLにして解放済みを示す
		return (0);
	}
	return (1);
}

/* parse_tokensのループ内でのトークン処理 */
static int	process_token_in_parse_loop(t_command **cmd_ptr,
		t_token **current_token_ptr, t_command **head_cmd_ptr)
{
	t_token_type	type;
	int				status;

	status = 1; // デフォルトは成功
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
		// 不明なトークンタイプまたはその他のケース (例: セミコロンなど未対応のメタ文字)
		*current_token_ptr = (*current_token_ptr)->next; // とりあえず進める
	return (status);
}

/* コマンド構造体の検証 */
static int	validate_command(t_command *head, t_token *tokens)
{
	if (head && head->args == NULL && head->redirects == NULL
		&& head->next == NULL && tokens->type == TOKEN_PIPE)
	{
		// `cmd |` のようなケースで `handle_pipe_token` がエラーを返し
		// `head` が `NULL` になっていることを期待。
		// もし `head` が `NULL` でない場合、それは不完全なコマンドかもしれない。
		// 現状の `handle_pipe_token` はエラー時に `*head_cmd = NULL` を行う。
		return (0);
	}
	return (1);
}

/* トークン列をコマンド構造体に変換するメインループ処理 */
static t_command	*parse_tokens_loop(t_token *tokens)
{
	t_command	*cmd;
	t_command	*head;
	t_token		*current;

	cmd = create_command();
	if (!cmd)
	{
		ft_putstr_fd((char *)"minishell: failed to create command structure\n", 2);
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

/* トークン列をコマンド構造体に変換 */
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
