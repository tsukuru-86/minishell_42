/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:30:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/13 18:18:35 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug_utils.h"

const char	*get_token_type_str(t_token_type type)
{
	static const char	*types[] = {[TOKEN_WORD] = "WORD",
	[TOKEN_SPACE] = "SPACE", [TOKEN_SINGLE_QUOTE] = "S_QUOTE",
	[TOKEN_DOUBLE_QUOTE] = "D_QUOTE",
	[TOKEN_S_QUOTED_WORD] = "S_QUOTED_WORD",
	[TOKEN_D_QUOTED_WORD] = "D_QUOTED_WORD",
	[TOKEN_EMPTY_QUOTED] = "EMPTY_QUOTED", [TOKEN_PIPE] = "PIPE",
	[TOKEN_REDIR_IN] = "REDIR_IN", [TOKEN_REDIR_OUT] = "REDIR_OUT",
	[TOKEN_REDIR_APPEND] = "REDIR_APPEND", [TOKEN_HEREDOC] = "HEREDOC",
	[TOKEN_ENV_VAR] = "ENV_VAR", [TOKEN_END] = "END"};

	if (type >= 0 && type < sizeof(types) / sizeof(types[0]) && types[type])
		return (types[type]);
	return ("UNKNOWN");
}

void	print_tokens_debug(const t_token *tokens, int debug)
{
	int	i;

	if (!debug || !tokens)
		return ;
	i = 0;
	while (tokens)
	{
		if (tokens->content)
		{
			printf("[DEBUG][token %d] content: \"%s\", type: %s\n", i,
				tokens->content, get_token_type_str(tokens->type));
		}
		else
		{
			printf("[DEBUG][token %d] content: (null), type: %s\n", i,
				get_token_type_str(tokens->type));
		}
		tokens = tokens->next;
		i++;
	}
}

void	print_args_debug(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		printf("[DEBUG][args %d] \"%s\"\n", i, args[i]);
		i++;
	}
}
