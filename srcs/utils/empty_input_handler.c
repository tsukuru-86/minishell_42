/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_input_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 06:33:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/09 01:59:13 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils/utils.h"

int	validate_input(char *input)
{
	int	i;

	if (!input)
		return (0);
	if (ft_strlen(input) == 0)
		return (0);
	i = 0;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t')
			return (1);
		i++;
	}
	return (0);
}

int	handle_empty_input(char *input)
{
	if (!validate_input(input))
		return (0);
	return (-1);
}

static int	process_tokenization(char *prepared)
{
	t_token	*tokens;
	int		status;

	tokens = tokenize(prepared, NULL);
	if (!tokens)
	{
		return (get_exit_status());
	}
	status = handle_tokens_and_parse(tokens);
	return (status);
}

void	process_valid_input(char *input, int *status)
{
	char	*prepared;

	prepared = ft_strdup(input);
	if (!prepared)
		return ;
	add_history(input);
	if (!validate_input(prepared))
	{
		free(prepared);
		*status = 0;
		return ;
	}
	*status = process_tokenization(prepared);
	free(prepared);
}
