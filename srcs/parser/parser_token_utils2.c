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
#include "utils/utils.h"

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

int	handle_interactive_heredoc(t_command *cmd, t_token *delimiter_token,
		t_command **head_cmd, t_token **current_token)
{
	if (!handle_heredoc(cmd, delimiter_token))
	{
		handle_heredoc_error(head_cmd);
		return (0);
	}
	*current_token = delimiter_token->next;
	return (1);
}

int	handle_noninteractive_heredoc(t_command *cmd, t_token *delimiter_token,
		t_command **head_cmd, t_token **current_token)
{
	if (!write_heredoc_from_stdin(cmd, delimiter_token))
	{
		handle_heredoc_error(head_cmd);
		return (0);
	}
	*current_token = delimiter_token->next;
	return (1);
}
