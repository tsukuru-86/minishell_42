/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_lookahead.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 16:45:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/16 00:28:39 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	handle_heredoc_with_lookahead(t_command *cmd, t_token **current_token,
		t_command **head_cmd)
{
	t_token	*lookahead;

	if (!cmd || !current_token || !*current_token || !head_cmd)
		return (0);
	lookahead = (*current_token)->next;
	if (lookahead)
		lookahead = lookahead->next;
	return (handle_heredoc_redirect(cmd, current_token, head_cmd));
}

static void	process_heredoc_delimiter(t_token *delimiter_token)
{
	char	*old;
	char	*new_word;
	size_t	len;
	char	quote;

	old = delimiter_token->content;
	len = ft_strlen(old);
	if (delimiter_token->type == TOKEN_D_QUOTED_WORD)
		quote = '"';
	else
		quote = '\'';
	new_word = (char *)malloc(len + 3);
	if (!new_word)
		return ;
	new_word[0] = quote;
	ft_memcpy(new_word + 1, old, len);
	new_word[len + 1] = quote;
	new_word[len + 2] = '\0';
	free(old);
	delimiter_token->content = new_word;
}

int	handle_heredoc_redirect(t_command *cmd, t_token **current_token,
		t_command **head_cmd)
{
	t_token	*delimiter_token;

	delimiter_token = (*current_token)->next;
	if (delimiter_token && (delimiter_token->type == TOKEN_D_QUOTED_WORD
			|| delimiter_token->type == TOKEN_S_QUOTED_WORD))
		process_heredoc_delimiter(delimiter_token);
	if (isatty(STDIN_FILENO))
		return (handle_interactive_heredoc(cmd, delimiter_token, head_cmd,
				current_token));
	else
		return (handle_noninteractive_heredoc(cmd, delimiter_token, head_cmd,
				current_token));
}
