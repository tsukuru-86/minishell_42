/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 08:59:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/20 08:59:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils/input_utils.h"

void	handle_heredoc_error(t_command **head_cmd)
{
	if (*head_cmd)
		free_command(*head_cmd);
	*head_cmd = NULL;
}

void	skip_to_delimiter(t_token **current_token, const char *delimiter)
{
	*current_token = (*current_token)->next;
	while (*current_token)
	{
		if ((*current_token)->type == TOKEN_WORD
			&& ft_strcmp((*current_token)->content, delimiter) == 0)
		{
			*current_token = (*current_token)->next;
			break ;
		}
		*current_token = (*current_token)->next;
	}
}
