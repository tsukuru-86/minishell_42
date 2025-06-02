/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 03:57:57 by muiida            #+#    #+#             */
/*   Updated: 2025/06/03 04:40:29 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

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

/* トークンタイプを文字列に変換 */
static const char	*token_type_to_string(t_token_type type)
{
	static const char	*type_names[] = {"WORD", "SPACE", "SINGLE_QUOTE",
		"DOUBLE_QUOTE", "S_QUOTED_WORD", "D_QUOTED_WORD", "PIPE",
		"REDIR_IN", "REDIR_OUT", "REDIR_APPEND", "HEREDOC", "ENV_VAR",
		"END"};

	if (type >= 0 && type < (int)(sizeof(type_names) / sizeof(type_names[0])))
		return (type_names[type]);
	return ("UNKNOWN");
}

/* デバッグ用：トークンの内容を表示 */
void	print_tokens(t_token *tokens)
{
	t_token	*current;
	int		i;

	printf("\n=== TOKENIZER OUTPUT ===\n");
	if (!tokens)
	{
		printf("No tokens generated\n");
		printf("========================\n\n");
		return ;
	}
	i = 0;
	current = tokens;
	while (current)
	{
		printf("Token[%d]: type=%s, content='%s'\n", i++,
			token_type_to_string(current->type),
			current->content ? current->content : "(null)");
		current = current->next;
	}
	printf("========================\n\n");
}

/* トークンリストの最後のトークンを取得 */
t_token	*get_last_token(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (NULL);
	current = tokens;
	while (current->next)
		current = current->next;
	return (current);
}
