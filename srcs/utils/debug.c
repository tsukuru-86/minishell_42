/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 00:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/16 22:54:58 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	debug_print_token(const t_token *token, bool is_debug)
{
	char	*type_str[18];

	type_str[0] = "TOKEN_WORD";
	type_str[1] = "TOKEN_SPACE";
	type_str[2] = "TOKEN_SINGLE_QUOTE";
	type_str[3] = "TOKEN_DOUBLE_QUOTE";
	type_str[4] = "TOKEN_S_QUOTED_WORD";
	type_str[5] = "TOKEN_D_QUOTED_WORD";
	type_str[6] = "TOKEN_PIPE";
	type_str[7] = "TOKEN_REDIR_IN";
	type_str[8] = "TOKEN_REDIR_OUT";
	type_str[9] = "TOKEN_REDIR_APPEND";
	type_str[10] = "TOKEN_HEREDOC";
	type_str[12] = "TOKEN_ENV_VAR";
	type_str[13] = "TOKEN_NEWLINE";
	type_str[14] = "TOKEN_END";
	type_str[15] = "TOKEN_INVALID";
	if (!is_debug)
		return ;
	if (token->content == NULL)
		printf("[NULL: ");
	else
		printf("[%s: ", token->content);
	printf("%s]\n", type_str[token->type]);
}

void	debug_print(const char *message, bool is_debug)
{
	if (is_debug)
		printf("%s\n", message);
}

void	debug_print_with_str(const char *prefix, const char *str, bool is_debug)
{
	if (is_debug)
		printf("%s'%s'\n", prefix, str);
}

void	debug_print_tokens(t_token *tokens, bool is_debug)
{
	t_token	*tmp;

	if (!is_debug)
		return ;
	if (!tokens)
	{
		printf("[DEBUG] Token list: NULL \n");
		return ;
	}
	else
		printf("[DEBUG] Token list:\n");
	tmp = tokens;
	while (tmp)
	{
		debug_print_token(tmp, is_debug);
		tmp = tmp->next;
	}
}

void	debug_print_command_args(char **args, bool is_debug)
{
	int	i;

	if (!is_debug || !args)
		return ;
	printf("[DEBUG] Command arguments:\n");
	i = 0;
	while (args[i])
	{
		printf("[DEBUG] args[%d]: '%s'\n", i, args[i]);
		i++;
	}
}
