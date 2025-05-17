/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_processing.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/16 15:16:45 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* トークンタイプがワード系の場合の処理 */
bool	process_word_token(t_token **current_token_ptr,
		char **accumulated_arg_ptr)
{
	char	*content;
	char	*new_arg;

	content = (*current_token_ptr)->content;
	if (*accumulated_arg_ptr == NULL)
		*accumulated_arg_ptr = ft_strdup(content);
	else
	{
		new_arg = ft_strjoin(*accumulated_arg_ptr, content);
		free(*accumulated_arg_ptr);
		*accumulated_arg_ptr = new_arg;
	}
	if (*accumulated_arg_ptr == NULL)
	{
		perror("minishell: ft_strdup/ft_strjoin failed in parser");
		return (false);
	}
	*current_token_ptr = (*current_token_ptr)->next;
	return (true);
}

/* スペーストークンの処理 */
bool	process_space_token(t_command *cmd_ptr, t_token **current_token_ptr,
		char **accumulated_arg_ptr)
{
	if (*accumulated_arg_ptr != NULL)
	{
		if (add_argument(cmd_ptr, *accumulated_arg_ptr) != RET_SUCCESS)
		{
			free(*accumulated_arg_ptr);
			*accumulated_arg_ptr = NULL;
			ft_printf_fd(2, "minishell: failed to add argument after space\n");
			return (false);
		}
		*accumulated_arg_ptr = NULL;
	}
	*current_token_ptr = (*current_token_ptr)->next;
	return (true);
}

bool	handle_error_token(t_token **current_token_ptr,
		char **accumulated_arg_ptr)
{
	if (*accumulated_arg_ptr)
		free(*accumulated_arg_ptr);
	*accumulated_arg_ptr = NULL;
	ft_printf_fd(2, "minishell: syntax error near unexpected token '%s'\n",
		(*current_token_ptr)->content);
	return (false);
}
