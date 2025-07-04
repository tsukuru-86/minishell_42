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
int	handle_quoted_token_creation(t_tokenizer_stat *stat, const char *input)
{
	t_token		*new_token;
	t_token		*last_token;
	char		*combined_content;
	t_token_type	token_type;

	if (!extract_quoted_string(stat, input, stat->word_buffer))
		return (0);
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
	token_type = stat->quote_type;
	if (should_mark_as_heredoc_delimiter(stat->tokens))
	{
		debug_print_with_str("DEBUG: marking as TOKEN_HEREDOC_DELIMITER", stat->word_buffer);
		token_type = TOKEN_HEREDOC_DELIMITER;
	}
	else
	{
		debug_print_with_str("DEBUG: not a heredoc delimiter, type is", "quoted_word");
	}
	new_token = safe_create_token(stat->word_buffer, token_type);
	if (!new_token)
		return (0);
	add_token_to_list(&stat->tokens, new_token);
	return (1);
}

/* メタ文字セグメントを処理してトークンを作成し、リストに追加 */
int	handle_meta_token_creation(t_tokenizer_stat *stat, const char *input)
{
	t_token	*new_token;

	new_token = create_meta_token(input, &stat->i_input);
	if (!new_token)
		return (0);
	add_token_to_list(&stat->tokens, new_token);
	return (1);
}

/* 生の単語を展開し、TOKEN_WORD トークンを作成 */
t_token	*create_expanded_word_token(char *raw_word, int *status)
{
	char	*expanded_content;
	t_token	*new_token;

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
	new_token = safe_create_token(expanded_content, TOKEN_WORD);
	free(expanded_content);
	if (!new_token)
		return (NULL);
	*status = 2;
	return (new_token);
}

/* ホワイトスペース文字をスキップする */
void	skip_whitespace(const char *input, int *i)
{
	while (input[*i] && (input[*i] == ' ' || input[*i] == '\t'
			|| input[*i] == '\n' || input[*i] == '\v' || input[*i] == '\f'
			|| input[*i] == '\r'))
		(*i)++;
}
