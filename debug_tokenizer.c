/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_tokenizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 14:19:35 by muiida            #+#    #+#             */
/*   Updated: 2025/06/18 14:19:49 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* デバッグ用tokenizer確認プログラム */
#include "minishell.h"

void	print_token_type(t_token_type type)
{
	switch (type)
	{
	case TOKEN_WORD:
		printf("WORD");
		break ;
	case TOKEN_SPACE:
		printf("SPACE");
		break ;
	case TOKEN_SINGLE_QUOTE:
		printf("S_QUOTE");
		break ;
	case TOKEN_DOUBLE_QUOTE:
		printf("D_QUOTE");
		break ;
	case TOKEN_S_QUOTED_WORD:
		printf("S_QUOTED_WORD");
		break ;
	case TOKEN_D_QUOTED_WORD:
		printf("D_QUOTED_WORD");
		break ;
	case TOKEN_PIPE:
		printf("PIPE");
		break ;
	case TOKEN_REDIR_IN:
		printf("REDIR_IN");
		break ;
	case TOKEN_REDIR_OUT:
		printf("REDIR_OUT");
		break ;
	case TOKEN_REDIR_APPEND:
		printf("REDIR_APPEND");
		break ;
	case TOKEN_HEREDOC:
		printf("HEREDOC");
		break ;
	case TOKEN_ENV_VAR:
		printf("ENV_VAR");
		break ;
	case TOKEN_NEWLINE:
		printf("NEWLINE");
		break ;
	case TOKEN_END:
		printf("END");
		break ;
	default:
		printf("UNKNOWN");
		break ;
	}
}

void	debug_tokens(t_token *tokens)
{
	t_token	*current;
	int		i;

	current = tokens;
	i = 0;
	printf("=== TOKEN DEBUG ===\n");
	while (current)
	{
		printf("Token[%d]: type=", i);
		print_token_type(current->type);
		printf(", content=\"%s\"\n", current->content);
		current = current->next;
		i++;
	}
	printf("==================\n");
}

int	main(int argc, char **argv)
{
	t_token *tokens;
	t_command cmd;

	if (argc != 2)
	{
		printf("Usage: %s \"command string\"\n", argv[0]);
		return (1);
	}

	memset(&cmd, 0, sizeof(t_command));
	cmd.last_status = 0;

	tokens = tokenize(argv[1], &cmd);
	if (tokens)
	{
		debug_tokens(tokens);
		free_tokens(tokens);
	}
	else
	{
		printf("Tokenize failed!\n");
	}
	return (0);
}