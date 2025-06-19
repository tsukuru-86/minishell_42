/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:20:26 by muiida            #+#    #+#             */
/*   Updated: 2025/06/18 23:55:03 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_all_pipe_input(void)
{
	char	*line;
	char	*result;
	char	*temp;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	line = get_next_line(0);
	while (line != NULL)
	{
		temp = ft_strjoin(result, line);
		free(result);
		free(line);
		if (!temp)
			return (NULL);
		result = temp;
		line = get_next_line(0);
	}
	return (result);
}
