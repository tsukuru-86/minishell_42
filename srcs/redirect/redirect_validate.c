/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_validate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 01:12:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/23 19:30:05 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

int	validate_input_redirect(t_redirect *current)
{
	if (!check_filename_limit(current->file))
		return (0);
	if (current->type == REDIR_IN)
		return (validate_input_redirect_in(current));
	return (1);
}
