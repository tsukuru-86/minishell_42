/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_handlers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 10:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/17 10:00:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/error_messages.h"
#include "minishell.h"
#include "parser.h"
#include "utils/debug.h"

/*
** @brief Handle REDIRECT type tokens.
** @param cmd_ptr Pointer to the current command structure
** @param current_token_ptr Pointer to the current token
** @param head_cmd_ptr Pointer to the head of the command list
** @return 1 on success, 0 on failure
*/
int	handle_redirect_type_tokens(t_command **cmd_ptr,
		t_token **current_token_ptr, t_command **head_cmd_ptr)
{
	int	status;

	dbg_printf("Handling REDIRECT token");
	status = handle_redirect_token(*cmd_ptr, current_token_ptr, head_cmd_ptr);
	return (status);
}

/*
** @brief Handle other tokens.
** @param current_token_ptr Pointer to the current token
** @param type Token type
** @return Always 1
*/
int	handle_other_tokens(t_token **current_token_ptr, t_token_type type)
{
	if (type == TOKEN_SPACE)
	{
		dbg_printf("Skipping SPACE token");
	}
	else if (type == TOKEN_NEWLINE)
	{
		dbg_printf("Skipping NEWLINE token");
	}
	else
	{
		dbg_printf("Skipping OTHER token");
	}
	*current_token_ptr = (*current_token_ptr)->next;
	return (1);
}
