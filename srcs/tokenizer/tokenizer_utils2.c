/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 05:37:47 by muiida            #+#    #+#             */
/*   Updated: 2025/06/03 04:40:29 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

/* トークナイザの状態を初期化。cmdがNULLの場合、新しいコマンド構造体を割り当てる */
int	init_tokenizer_stat(t_tokenizer_stat *stat, t_command *cmd)
{
	stat->tokens = NULL;
	stat->i_input = 0;
	stat->needs_cmd_free = false;
	if (!cmd)
	{
		stat->cmd = (t_command *)malloc(sizeof(t_command));
		if (!stat->cmd)
			return (0);
		ft_memset(stat->cmd, 0, sizeof(t_command));
		stat->cmd->last_status = 0;
		stat->needs_cmd_free = true;
	}
	else
		stat->cmd = cmd;
	return (1);
}

/* 単語セグメントを抽出し、展開し、トークンを作成してリストに追加 */
int	handle_word_token_creation(t_tokenizer_stat *stat, const char *input)
{
	char		buf[1024];
	int			buf_len;
	char		quote_c;
	t_token		*new_token;
	t_token_type	token_type;
	int			i;

	i = stat->i_input;
	buf_len = 0;
	if (is_quote(input[i]))
	{
		quote_c = input[i];
		i++;
		while (input[i] && input[i] != quote_c)
		{
			if (buf_len < 1023)
				buf[buf_len++] = input[i];
			i++;
		}
		if (input[i] == quote_c)
			i++;
		token_type = (quote_c == '\'') ? TOKEN_S_QUOTED_WORD : TOKEN_D_QUOTED_WORD;
	}
	else
	{
		while (input[i] && !is_delimiter(input[i]) && !is_quote(input[i])
			&& !is_meta(input[i]))
		{
			if (buf_len < 1023)
				buf[buf_len++] = input[i];
			i++;
		}
		token_type = TOKEN_WORD;
	}
	buf[buf_len] = '\0';
	stat->i_input = i;
	if (token_type == TOKEN_D_QUOTED_WORD)
	{
		char *expanded = expand_env_vars(buf, 1);
		if (!expanded)
			return (0);
		new_token = create_token(expanded, token_type);
		free(expanded);
	}
	else if (token_type == TOKEN_WORD)
	{
		char *expanded = expand_env_vars(buf, 0);
		if (!expanded)
			return (0);
		new_token = create_token(expanded, token_type);
		free(expanded);
	}
	else
		new_token = create_token(buf, token_type);
	if (!new_token)
		return (0);
	add_token_to_list(&stat->tokens, new_token);
	return (1);
}

/* トークンリストと内部で割り当てられたコマンド構造体を解放し、NULLを返す */
t_token	*cleanup_and_return_null(t_tokenizer_stat *stat, char *input)
{
	if (!handle_word_token_creation(stat, input))
		free_tokens(stat->tokens);
	stat->tokens = NULL;
	if (stat->needs_cmd_free && stat->cmd)
	{
		free(stat->cmd);
		stat->cmd = NULL;
	}
	return (NULL);
}

/* トークナイズ処理の最後に呼び出され、内部で割り当てられたコマンド構造体を解放 */
void	finalize_tokenizer(t_tokenizer_stat *stat)
{
	if (stat->needs_cmd_free && stat->cmd)
	{
		free(stat->cmd);
		stat->cmd = NULL;
	}
}
