/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:20:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/12 20:22:11 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils/string_utils.h"

char	*prepare_input(char *input)
{
	char	*trimmed;
	char	*replaced;

	trimmed = ft_strtrim(input, " \t\n\r");
	if (!trimmed || !*trimmed)
	{
		if (trimmed)
			free(trimmed);
		return (NULL);
	}
	replaced = str_replace_backslash_n(trimmed);
	free(trimmed);
	return (replaced);
}
