/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 18:42:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/26 04:01:41 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"

int	get_exit_status(t_command *cmd)
{
	t_env	*status_node;

	(void)cmd;
	status_node = get_env_node("?");
	if (!status_node || !status_node->value)
		return (0);
	return (ft_atoi(status_node->value));
}

void	set_exit_status(t_command *cmd, int status)
{
	char	*status_str;

	(void)cmd;
	status_str = ft_itoa(status);
	if (!status_str)
		return ;
	set_env_node("?", status_str);
	free(status_str);
}
