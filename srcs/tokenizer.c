/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 04:53:10 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/23 21:47:27 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "tokenizer.h"

/* クォートされたセグメントを処理してトークンを作成し、リストに追加 */
static int	handle_quoted_token_creation(t_tokenizer_stat *vars,
		const char *input)
{
	t_token_type	quote_type;
	t_token			*new_token;

	if (!extract_quoted_string(input, &vars->i, vars->word_buffer, &quote_type,
			vars->cmd_instance))
		return (0);
	new_token = create_token(vars->word_buffer, quote_type);
	if (!new_token)
		return (0);
	add_token_to_list(&vars->tokens, new_token);
	return (1);
}

/* メタ文字セグメントを処理してトークンを作成し、リストに追加 */
static int	handle_meta_token_creation(t_tokenizer_stat *vars,
		const char *input)
{
	t_token	*new_token;

	new_token = create_meta_token(input, &vars->i);
	if (!new_token)
		return (0);
	add_token_to_list(&vars->tokens, new_token);
	return (1);
}

/* 入力から生の単語を抽出し、バッファに格納 */
int	extract_raw_word(const char *input, int *i, char *word_buffer)
{
	int	word_i;

	word_i = 0;
	while (input[*i] && !is_delimiter(input[*i]) && !is_quote(input[*i])
		&& !is_meta(input[*i]))
	{
		if (word_i < 1024 - 1)
			word_buffer[word_i++] = input[*i];
		(*i)++;
	}
	word_buffer[word_i] = '\0';
	return (word_i);
}

/* 生の単語を展開し、TOKEN_WORD トークンを作成 */
t_token	*create_expanded_word_token(char *raw_word, t_command *cmd, int *status)
{
	char	*expanded_content;
	t_token	*new_token;

	*status = 0;
	if (ft_strlen(raw_word) == 0)
	{
		*status = 1;
		return (NULL);
	}
	expanded_content = expand_env_vars(raw_word, 0, cmd);
	if (!expanded_content)
		return (NULL);
	new_token = create_token(expanded_content, TOKEN_WORD);
	free(expanded_content);
	if (!new_token)
		return (NULL);
	*status = 2;
	return (new_token);
}

/* 入力文字列をトークンに分割 */
t_token	*tokenize(char *input, t_command *cmd_param)
{
	t_tokenizer_stat	vars;

	if (!init_tokenizer_stat(&vars, cmd_param))
		return (NULL);
	if (!input)
	{
		finalize_tokenizer(&vars);
		return (NULL);
	}
	while (input[vars.i])
	{
		while (input[vars.i] && ft_isspace(input[vars.i]))
			vars.i++;
		if (!input[vars.i])
			break ;
		if (is_quote(input[vars.i]))
		{
			if (!handle_quoted_token_creation(&vars, input))
				return (cleanup_and_return_null(&vars, input));
		}
		else if (is_meta(input[vars.i]))
		{
			if (!handle_meta_token_creation(&vars, input))
				return (cleanup_and_return_null(&vars, input));
		}
		else
		{
			if (!handle_word_token_creation(&vars, input))
				return (cleanup_and_return_null(&vars, input));
			finalize_tokenizer(&vars);
		}
	}
	return (vars.tokens);
}
