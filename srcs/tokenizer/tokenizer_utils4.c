/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 22:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/11 07:13:11 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* トークンタイプを文字列に変換 */
static const char	*token_type_to_string(t_token_type type)
{
	static const char	*type_names[] = {"WORD", "SPACE", "SINGLE_QUOTE",
		"DOUBLE_QUOTE", "S_QUOTED_WORD", "D_QUOTED_WORD", "EMPTY_QUOTED",
		"PIPE", "REDIR_IN", "REDIR_OUT", "REDIR_APPEND", "HEREDOC",
		"ENV_VAR", "END"};

	if (type >= 0 && type < (int)(sizeof(type_names) / sizeof(type_names[0])))
		return (type_names[type]);
	return ("UNKNOWN");
}

/* デバッグ用：トークンの内容を表示 */
void	print_tokens(t_token *tokens)
{
	t_token		*current;
	int			i;
	const char	*content_str;

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
		if (current->content)
			content_str = current->content;
		else
			content_str = "(null)";
		printf("Token[%d]: type=%s, content='%s'\n", i++,
			token_type_to_string(current->type), content_str);
		current = current->next;
	}
	printf("========================\n\n");
}
