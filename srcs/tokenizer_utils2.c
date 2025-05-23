/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 05:37:47 by muiida            #+#    #+#             */
/*   Updated: 2025/05/23 20:45:48 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		extract_raw_word(const char *input, int *i, char *word_buffer);

/* Check if character is a whitespace */
int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

/* トークナイザの状態を初期化。cmd_paramがNULLの場合、新しいコマンド構造体を割り当てる */
int	init_tokenizer_stat(t_tokenizer_stat *vars, t_command *cmd_param)
{
	vars->tokens = NULL;
	vars->i = 0;
	vars->needs_cmd_free = 0;
	if (!cmd_param)
	{
		vars->cmd_instance = (t_command *)malloc(sizeof(t_command));
		if (!vars->cmd_instance)
			return (0);
		ft_memset(vars->cmd_instance, 0, sizeof(t_command));
		vars->cmd_instance->last_status = 0;
		vars->needs_cmd_free = 1;
	}
	else
		vars->cmd_instance = cmd_param;
	return (1);
}

/* 単語セグメントを抽出し、展開し、トークンを作成してリストに追加 */
int	handle_word_token_creation(t_tokenizer_stat *vars, const char *input)
{
	t_token	*new_token;
	int		raw_word_len;
	int		expansion_status;

	raw_word_len = extract_raw_word(input, &vars->i, vars->word_buffer);
	if (raw_word_len == 0)
		return (1);
	new_token = create_expanded_word_token(vars->word_buffer,
			vars->cmd_instance, &expansion_status);
	if (expansion_status == 0)
		return (0);
	if (expansion_status == 1)
		return (1);
	add_token_to_list(&vars->tokens, new_token);
	return (1);
}

/* トークンリストと内部で割り当てられたコマンド構造体を解放し、NULLを返す */
t_token	*cleanup_and_return_null(t_tokenizer_stat *vars, char *input)
{
	if (!handle_word_token_creation(vars, input))
		free_tokens(vars->tokens);
	vars->tokens = NULL;
	if (vars->needs_cmd_free && vars->cmd_instance)
	{
		free(vars->cmd_instance);
		vars->cmd_instance = NULL;
	}
	return (NULL);
}

/* トークナイズ処理の最後に呼び出され、内部で割り当てられたコマンド構造体を解放 */
void	finalize_tokenizer(t_tokenizer_stat *vars)
{
	if (vars->needs_cmd_free && vars->cmd_instance)
	{
		free(vars->cmd_instance);
		vars->cmd_instance = NULL;
	}
}