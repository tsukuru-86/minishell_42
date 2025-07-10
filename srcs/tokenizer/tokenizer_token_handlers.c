/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_token_handlers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 21:35:00 by muiida           #+#    #+#             */
/*   Updated: 2025/05/24 21:35:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
#include "tokenizer.h"

/* 入力から生の単語を抽出し、バッファに格納 */
int	extract_raw_word(const char *input, int *i, char *word_buffer)
{
	int	word_i;

	word_i = 0;
	while (input[*i] && !is_delimiter(input[*i]) && !is_quote(input[*i])
		&& !is_meta(input[*i]) && input[*i] != '\n')
	{
		if (word_i < 1024 - 1)
			word_buffer[word_i++] = input[*i];
		(*i)++;
	}
	word_buffer[word_i] = '\0';
	return (word_i);
}

/* クォートされたセグメントを処理してトークンを作成し、リストに追加 */
static int	handle_quoted_token_merge(t_tokenizer_stat *stat, const char *input)
{
	t_token	*last_token;
	char	*combined_content;

	last_token = get_last_token(stat->tokens);
	if (last_token && (last_token->type == TOKEN_S_QUOTED_WORD
			|| last_token->type == TOKEN_D_QUOTED_WORD)
		&& !is_delimiter(input[stat->i_input - 1]) && input[stat->i_input
			- 1] != '\n')
	{
		combined_content = ft_strjoin(last_token->content, stat->word_buffer);
		if (!combined_content)
			return (0);
		free(last_token->content);
		last_token->content = combined_content;
		return (1);
	}
	return (0);
}

int	handle_quoted_token_creation(t_tokenizer_stat *stat, const char *input)
{
	t_token			*new_token;
	t_token_type	token_type;

	if (!extract_quoted_string(stat, input, stat->word_buffer))
		return (0);
	if (handle_quoted_token_merge(stat, input))
		return (1);
	token_type = stat->quote_type;
	if (should_mark_as_heredoc_delimiter(stat->tokens))
		token_type = TOKEN_HEREDOC_DELIMITER;
	new_token = safe_create_token(stat->word_buffer, token_type);
	if (!new_token)
		return (0);
	add_token_to_list(&stat->tokens, new_token);
	return (1);
}

/* 生の単語を展開する（分割後） */
char	*expand_word_content(char *raw_word, int *status)
{
	char	*expanded_content;

	*status = 0;
	if (ft_strlen(raw_word) == 0)
	{
		*status = 1;
		return (NULL);
	}
	expanded_content = expand_env_vars(raw_word, 0);
	if (!expanded_content)
		return (NULL);
	if (ft_strlen(expanded_content) == 0)
	{
		free(expanded_content);
		*status = 1;
		return (NULL);
	}
	return (expanded_content);
}

/* 展開済み内容からTOKEN_WORDトークンを生成（分割後） */
t_token	*create_expanded_word_token(char *raw_word, int *status)
{
	char	*expanded_content;
	t_token	*new_token;

	expanded_content = expand_word_content(raw_word, status);
	if (!expanded_content)
		return (NULL);
	new_token = safe_create_token(expanded_content, TOKEN_WORD);
	free(expanded_content);
	if (!new_token)
		return (NULL);
	*status = 2;
	return (new_token);
}
