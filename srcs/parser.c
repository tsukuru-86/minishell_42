/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:13:41 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/15 07:18:04 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
	{
		perror("minishell: malloc failed in initialize t_command list");
		return (NULL);
	}
	cmd = malloc(sizeof(t_command));
	if (cmd == NULL)
		return (NULL);
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	cmd->pipe.read_fd = STDIN_FILENO;
	cmd->pipe.write_fd = STDOUT_FILENO;
	return (cmd);
}

static int	parse_token_to_args(t_command *cmd, t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens && tokens->type == TOKEN_WORD)
	{
		cmd->args = (char **)realloc(cmd->args, sizeof(char *) * (i + 2));
		if (!cmd->args)
			return (0);
		cmd->args[i] = strdup(tokens->content);
		if (!cmd->args[i])
			return (0);
		tokens = tokens->next;
		i++;
	}
	cmd->args[i] = NULL;
	return (1);
}

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*cmd;

	cmd = init_command();
	if (cmd == NULL)
		return (NULL);
	if (!parse_token_to_args(cmd, tokens))
	{
		free_command(cmd);
		return (NULL);
	}
	return (cmd);
}
