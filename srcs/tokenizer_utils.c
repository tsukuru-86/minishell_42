/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 03:57:57 by muiida            #+#    #+#             */
/*   Updated: 2025/05/12 04:18:19 by muiida           ###   ########.fr       */
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
static void	print_tokens(t_token *tokens)
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

/* Extract quoted string and handle environment variable expansion */
static char	*extract_quoted_string(char *input, int *i, char quote)
{
	char	*content;
	int		start;
	int		len;
	char	*expanded;

	start = *i + 1;
	len = 0;
	while (input[start + len] && input[start + len] != quote)
		len++;
	if (!input[start + len])
		return (NULL);
	content = ft_substr(input, start, len);
	if (!content)
		return (NULL);
	if (quote == '"')
	{
		expanded = expand_env_vars(content, 1);
		free(content);
		content = expanded;
	}
	*i = start + len + 1;
	return (content);
}
