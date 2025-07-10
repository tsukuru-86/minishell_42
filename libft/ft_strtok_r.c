/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok_r.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 05:57:21 by muiida            #+#    #+#             */
/*   Updated: 2025/07/11 06:00:04 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is_delim(char c, const char *delim)
{
	int	i;

	i = 0;
	while (delim[i])
	{
		if (c == delim[i])
			return (1);
		i++;
	}
	return (0);
}

static void	ft_split_token(char **saveptr, int i)
{
	(*saveptr)[i] = '\0';
	*saveptr = &(*saveptr)[i + 1];
}

static int	ft_is_token_valid(char *token)
{
	return (token[0] != '\0');
}

static char	*ft_next_token(char **saveptr, const char *delim, char **token)
{
	int	i;

	i = 0;
	while ((*saveptr)[i])
	{
		if (ft_is_delim((*saveptr)[i], delim))
		{
			ft_split_token(saveptr, i);
			if (ft_is_token_valid(*token))
				return (*token);
			*token = *saveptr;
			i = -1;
		}
		i++;
	}
	*saveptr = NULL;
	if (!ft_is_token_valid(*token))
		return (NULL);
	return (*token);
}

char	*ft_strtok_r(char *str, const char *delim, char **saveptr)
{
	char	*token;

	if (str)
		*saveptr = str;
	if (!*saveptr || !delim)
		return (NULL);
	token = *saveptr;
	return (ft_next_token(saveptr, delim, &token));
}
