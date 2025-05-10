/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 01:31:16 by muiida           #+#    #+#             */
/*   Updated: 2025/05/11 01:31:16 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Replace whitespace characters (tab and newline) with space */
static void	replace_whitespace(char *str)
{
	while (*str)
	{
		if (*str == '\t' || *str == '\n')
			*str = ' ';
		str++;
	}
}

/* Split input line into tokens by space */
char	**split_command(char *input)
{
	char	**tokens;

	if (!input)
		return (NULL);
	replace_whitespace(input);
	tokens = ft_split(input, ' ');
	if (!tokens)
		return (NULL);
	return (tokens);
}
