/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:37:04 by muiida            #+#    #+#             */
/*   Updated: 2025/05/18 03:18:59 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_minishell	**shell_state_holder(void)

{
	static t_minishell	*shell = NULL;
	static bool			initialized = false;

	if (!initialized)
	{
		shell = (t_minishell *)ft_calloc(1, sizeof(t_minishell));
		if (!shell)
		{
			perror("ft_calloc");
			exit(EXIT_FAILURE);
		}
		initialized = 1;
	}
	return (&shell);
}
