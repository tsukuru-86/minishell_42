/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand_name.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 04:10:00 by muiida           #+#    #+#             */
/*   Updated: 2025/05/12 04:10:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Extract environment variable name from string */
char	*extract_env_name(const char *str)
{
	int		i;
	char	name[1024];

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		name[i] = str[i];
		i++;
	}
	name[i] = '\0';
	return (ft_strdup(name));
}
