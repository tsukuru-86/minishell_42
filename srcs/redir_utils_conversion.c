/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils_conversion.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 01:26:13 by muiida            #+#    #+#             */
/*   Updated: 2025/05/15 01:34:58 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_redir_type	convert_token_type_to_redir_type(t_token_type token_redir_type)
{
	if (token_redir_type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	else if (token_redir_type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	else if (token_redir_type == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	else if (token_redir_type == TOKEN_HEREDOC)
		return (REDIR_HEREDOC);
	return (-1);
}
