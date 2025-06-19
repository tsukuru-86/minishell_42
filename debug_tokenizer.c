/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_tokenizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 14:19:35 by muiida            #+#    #+#             */
/*   Updated: 2025/06/19 17:33:24 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* デバッグ用tokenizer確認プログラム */
#include "minishell.h"

void	print_token_type(t_token_type type)
{
	if (type == TOKEN_WORD)
		debug_print("WORD", DEBUG_ENABLED);
	else if (type == TOKEN_SPACE)
		debug_print("SPACE", DEBUG_ENABLED);
	else if (type == TOKEN_SINGLE_QUOTE)
		debug_print("S_QUOTE", DEBUG_ENABLED);
	else if (type == TOKEN_DOUBLE_QUOTE)
		debug_print("D_QUOTE", DEBUG_ENABLED);
	else if (type == TOKEN_S_QUOTED_WORD)
		debug_print("S_QUOTED_WORD", DEBUG_ENABLED);
	else if (type == TOKEN_D_QUOTED_WORD)
		debug_print("D_QUOTED_WORD", DEBUG_ENABLED);
	else if (type == TOKEN_PIPE)
		debug_print("PIPE", DEBUG_ENABLED);
	else if (type == TOKEN_REDIR_IN)
		debug_print("REDIR_IN", DEBUG_ENABLED);
	else if (type == TOKEN_REDIR_OUT)
		debug_print("REDIR_OUT", DEBUG_ENABLED);
	else if (type == TOKEN_REDIR_APPEND)
		debug_print("REDIR_APPEND", DEBUG_ENABLED);
	else if (type == TOKEN_HEREDOC)
		debug_print("HEREDOC", DEBUG_ENABLED);
	else if (type == TOKEN_ENV_VAR)
		debug_print("ENV_VAR", DEBUG_ENABLED);
	else if (type == TOKEN_NEWLINE)
		debug_print("NEWLINE", DEBUG_ENABLED);
	else if (type == TOKEN_END)
		debug_print("END", DEBUG_ENABLED);
	else
		debug_print("UNKNOWN", DEBUG_ENABLED);
}
