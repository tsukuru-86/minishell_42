/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 04:53:10 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/12 04:18:47 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 新しいトークンを作成 */
t_token	*create_token(char *content, t_token_type type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->content = ft_strdup(content);
	if (!token->content)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

/* トークンをリストに追加*/
static void	add_token(t_token **token, t_token *new_token)
{
	t_token	*current;

	if (!(*token))
	{
		*token = new_token;
		return ;
	}
	current = *token;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

/* クォートされた文字列を抽出 */
int	extract_quoted_string(char *input, int *i, char *word,
		t_token_type *type, t_command *cmd)
{
	char	quote_char;
	int		word_i;
	char	*expanded;

	quote_char = input[*i];
	(*i)++; // クォート文字をスキップ
	word_i = 0;
	*type = (quote_char == '\'') ? TOKEN_SINGLE_QUOTE : TOKEN_DOUBLE_QUOTE;
	while (input[*i] && input[*i] != quote_char)
	{
		word[word_i++] = input[*i];
		(*i)++;
	}
	if (input[*i] == quote_char)
	{
		(*i)++; // 終端のクォート文字をスキップ
		word[word_i] = '\0';
		// 環境変数の展開（シングルクォート内では展開しない）
		if (*type == TOKEN_DOUBLE_QUOTE)
		{
			expanded = expand_env_vars(word, 1, cmd);
			if (!expanded)
				return (0);
			ft_strlcpy(word, expanded, 1024);
			free(expanded);
		}
		return (1);
	}
	return (0); // クォートが閉じられていない
}

/* 入力文字列をトークンに分割 */
t_token	*tokenize(char *input, t_command *cmd)
{
	t_token			*tokens;
	char			word[1024];
	int				i;
	int				word_i;
	t_token_type	type;
	t_token			*new_token;
	char			*expanded;
	int				need_free_cmd;

	tokens = NULL;
	need_free_cmd = 0;
	if (!cmd)
	{
		cmd = (t_command *)malloc(sizeof(t_command));
		if (!cmd)
			return (NULL);
		ft_memset(cmd, 0, sizeof(t_command));
		cmd->last_status = 0;
		need_free_cmd = 1;
	}
	i = 0;
	while (input[i])
	{
		// スペースをスキップ
		if (is_delimiter(input[i]))
		{
			i++;
			continue ;
		}
		// クォートの処理
		if (is_quote(input[i]))
		{
			if (!extract_quoted_string(input, &i, word, &type, cmd))
			{
				// クォートが閉じられていない場合のエラー処理
				free_tokens(tokens);
				return (NULL);
			}
			new_token = create_token(word, type);
			if (!new_token)
			{
				free_tokens(tokens);
				return (NULL);
			}
			add_token(&tokens, new_token);
			continue ;
		}
		// メタ文字の処理
		if (is_meta(input[i]))
		{
			new_token = create_meta_token(input, &i);
			if (!new_token)
			{
				free_tokens(tokens);
				return (NULL);
			}
			add_token(&tokens, new_token);
			continue ;
		}
		// 通常の単語を抽出
		word_i = 0;
		while (input[i] && !is_delimiter(input[i]) && !is_quote(input[i])
			&& !is_meta(input[i]))
			word[word_i++] = input[i++];
		word[word_i] = '\0';
		// トークンを作成して追加
		if (word_i > 0)
		{
			// 環境変数の展開
			expanded = expand_env_vars(word, 0, cmd);
			if (!expanded)
			{
				free_tokens(tokens);
				return (NULL);
			}
			new_token = create_token(expanded, TOKEN_WORD);
			free(expanded);
			if (!new_token)
			{
				free_tokens(tokens);
				return (NULL);
			}
			add_token(&tokens, new_token);
		}
	}
	if (need_free_cmd)
		free(cmd);
	return (tokens);
}
