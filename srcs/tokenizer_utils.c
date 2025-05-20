/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 03:57:57 by muiida            #+#    #+#             */
/*   Updated: 2025/05/23 02:09:24 by muiida           ###   ########.fr       */
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
