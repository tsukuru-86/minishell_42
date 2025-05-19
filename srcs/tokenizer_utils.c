/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 03:57:57 by muiida            #+#    #+#             */
/*   Updated: 2025/05/13 19:15:43 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_delimiter(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_meta(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/* メタ文字のトークンタイプを判定 */
t_token_type	get_meta_type(char *input, int *i)
{
	if (input[*i] == '|')
	{
		(*i)++;
		return (TOKEN_PIPE);
	}
	else if (input[*i] == '<')
	{
		(*i)++;
		return (TOKEN_REDIR_IN);
	}
	else if (input[*i] == '>')
	{
		(*i)++;
		if (input[*i] == '>')
		{
			(*i)++;
			return (TOKEN_REDIR_APPEND);
		}
		return (TOKEN_REDIR_OUT);
	}
	return (TOKEN_WORD); // エラー防止のためのデフォルト値
}

/* メタ文字トークンを作成 */
t_token	*create_meta_token(char *input, int *i)
{
	t_token_type	type;
	char			meta_str[3];
	int				len;

	len = 0;
	meta_str[len++] = input[*i];
	type = get_meta_type(input, i);
	if (type == TOKEN_REDIR_APPEND)
		meta_str[len++] = '>';
	meta_str[len] = '\0';
	return (create_token(meta_str, type));
}

/* トークンリストの解放 */
void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
}

/* デバッグ用：トークンの内容を表示 */
void	print_tokens(t_token *tokens)
{
	t_token	*current;
	int		i;

	i = 0;
	current = tokens;
	while (current)
	{
		printf("Token %d: [%s] (type: %d)\n", i++, current->content,
			current->type);
		current = current->next;
	}
}

