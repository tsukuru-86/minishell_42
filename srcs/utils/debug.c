/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 00:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/03 04:35:43 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	debug_print_token(const t_token *token)
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
	type_str[11] = "TOKEN_HEREDOC_DELIMITER";
	type_str[12] = "TOKEN_ENV_VAR";
	type_str[13] = "TOKEN_EMPTY_VAR";
	type_str[14] = "TOKEN_NEWLINE";
	type_str[15] = "TOKEN_END";
	if (!DEBUG_ENABLED)
		return ;
	if (token->content == NULL)
		debug_print("[NULL: ");
	else
		ft_printf_fd(STDERR_FILENO, "[%s: ", token->content);
	ft_printf_fd(STDERR_FILENO, "%s]\n", type_str[token->type]);
}

void	debug_print(const char *message)
{
	if (DEBUG_ENABLED)
		ft_printf_fd(STDERR_FILENO, "%s\n", message);
}

void	debug_print_with_str(const char *prefix, const char *str)
{
	if (DEBUG_ENABLED)
		ft_printf_fd(STDERR_FILENO, "%s'%s'\n", prefix, str);
}

void	debug_print_tokens(t_token *tokens)
{
	t_token	*tmp;

	if (!DEBUG_ENABLED)
		return ;
	if (!tokens)
	{
		debug_print("[DEBUG] Token list: NULL \n");
		return ;
	}
	else
		debug_print("[DEBUG] Token list:\n");
	tmp = tokens;
	while (tmp)
	{
		debug_print_token(tmp);
		tmp = tmp->next;
	}
}

void	debug_print_command_args(char **args)
{
	int	i;

	if (!DEBUG_ENABLED || !args)
		return ;
	debug_print("[DEBUG] Command arguments:\n");
	i = 0;
	while (args[i])
	{
		debug_print_with_int("[DEBUG] i=", i);
		debug_print_with_str("args[i]=", args[i]);
		i++;
	}
}

void	print_commands_debug(t_command *commands)
{
	t_command	*current_cmd;
	int			cmd_idx;
	t_redirect	*current_redir;
	int			redir_idx;

	if (!DEBUG_ENABLED)
		return ;
	if (!commands)
	{
		debug_print("[DEBUG] Command list: NULL\n");
		return ;
	}
	debug_print("[DEBUG] Command list:\n");
	cmd_idx = 0;
	current_cmd = commands;
	while (current_cmd)
	{
		debug_print_with_int("[DEBUG] Command", cmd_idx);
		debug_print_command_args(current_cmd->args);

		current_redir = current_cmd->redirects;
		redir_idx = 0;
		while (current_redir)
		{
			debug_print_with_int("[DEBUG]   Redirect", redir_idx);
			debug_print_with_int("[DEBUG]     Type", current_redir->type);
			debug_print_with_str("[DEBUG]     File", current_redir->file);
			current_redir = current_redir->next;
			redir_idx++;
		}
		current_cmd = current_cmd->next;
		cmd_idx++;
	}
}
