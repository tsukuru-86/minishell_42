/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_input_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 06:33:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/14 06:52:33 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils/input_utils.h"

int	handle_tokens_and_parse(t_token *tokens);

int	handle_empty_input(char *input)
{
	int	i;
	int	is_empty;

	if (!input || !*input)
		return (127);
	i = 0;
	is_empty = 1;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t')
		{
			is_empty = 0;
			break ;
		}
		i++;
	}
	if (is_empty)
		return (0);
	return (-1);
}

void	process_valid_input(char *input, int *status)
{
	t_token	*tokens;
	char	*prepared;

	prepared = prepare_input(input);
	if (!prepared)
		return ;
	add_history(input);
	tokens = tokenize(prepared, NULL);
	free(prepared);
	if (!tokens)
	{
		*status = 2;
		return ;
	}
	*status = handle_tokens_and_parse(tokens);
}
