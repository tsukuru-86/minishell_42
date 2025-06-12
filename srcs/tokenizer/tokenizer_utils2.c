/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 05:37:47 by muiida            #+#    #+#             */
/*   Updated: 2025/06/12 15:45:41 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"
#include <stdio.h>

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

/* クォートされた文字列を抽出する */
static int	extract_quoted_content(const char *input, int *i, char *buf,
		int *buf_len)
{
	char	quote_c;
	int		start;
	int		ret;

	quote_c = input[*i];
	start = *i;
	(*i)++;
	*buf_len = 0;
	while (input[*i] && input[*i] != quote_c)
	{
		if (*buf_len < 1023)
			buf[(*buf_len)++] = input[*i];
		(*i)++;
	}
	if (input[*i] == quote_c)
		(*i)++;
	if (*buf_len == 0 && input[start] == quote_c && input[*i - 1] == quote_c)
		ret = TOKEN_EMPTY_QUOTED;
	else if (quote_c == '\'')
		ret = TOKEN_S_QUOTED_WORD;
	else
		ret = TOKEN_D_QUOTED_WORD;
	buf[*buf_len] = '\0';
	return (ret);
}

/* 通常の単語を抽出する */
static void	extract_word_content(const char *input, int *i, char *buf,
		int *buf_len)
{
	while (input[*i] && !is_delimiter(input[*i]) && !is_quote(input[*i])
		&& !is_meta(input[*i]))
	{
		if (*buf_len < 1023)
			buf[(*buf_len)++] = input[*i];
		(*i)++;
	}
}

/* トークンを作成し、必要に応じて環境変数を展開する */
static t_token	*create_expanded_token(char *buf, t_token_type token_type)
{
	char	*expanded;
	t_token	*new_token;

	// const char	*type_str = "UNKNOWN";
	// if (token_type == 0)
	// 	type_str = "TOKEN_WORD";
	// else if (token_type == 4)
	// 	type_str = "TOKEN_S_QUOTED_WORD";
	// else if (token_type == 5)
	// 	type_str = "TOKEN_D_QUOTED_WORD";
	// printf("[DEBUG] before create_expanded_token: token_type=%s, buf=%s\n",
	// type_str, buf);
	if (token_type == TOKEN_D_QUOTED_WORD)
	{
		expanded = expand_env_vars(buf, 1);
		if (!expanded)
			return (NULL);
		// printf("create_expanded_token: expand_env_vars: %s->%s\n", buf,
		// expanded);
		new_token = create_token(expanded, token_type);
		// printf("create_expanded_token: create_token: %s->%s\n",
		// expanded,new_token->content);
		free(expanded);
	}
	else if (token_type == TOKEN_WORD)
	{
		expanded = expand_env_vars(buf, 0);
		if (!expanded)
			return (NULL);
		new_token = create_token(expanded, token_type);
		free(expanded);
	}
	else
		new_token = create_token(buf, token_type);
	return (new_token);
}

/* 単語セグメントを抽出し、展開し、トークンを作成してリストに追加 */
int	handle_word_token_creation(t_tokenizer_stat *stat, const char *input)
{
	char			buf[1024];
	int				buf_len;
	t_token			*new_token;
	t_token_type	token_type;
	int				i;

	// const char		*type_str = "UNKNOWN";
	i = stat->i_input;
	buf_len = 0;
	if (is_quote(input[i]))
		token_type = extract_quoted_content(input, &i, buf, &buf_len);
	else
	{
		extract_word_content(input, &i, buf, &buf_len);
		token_type = TOKEN_WORD;
	}
	buf[buf_len] = '\0';
	stat->i_input = i;
	new_token = create_expanded_token(buf, token_type);
	add_token_to_list(&stat->tokens, new_token);
	// if (new_token->type == 0)
	// 	type_str = "TOKEN_WORD";
	// else if (new_token->type == 4)
	// 	type_str = "TOKEN_S_QUOTED_WORD";
	// else if (new_token->type == 5)
	// 	type_str = "TOKEN_D_QUOTED_WORD";
	// printf("[DEBUG] after create_expanded_token: token_type=%s,
	// new_token->content = %
	// s\n ", type_str, new_token->content);
	return (1);
}
