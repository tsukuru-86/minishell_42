/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_input_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 06:33:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/14 13:59:33 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils/input_utils.h"

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

int		handle_tokens_and_parse(t_token *tokens);

int	handle_empty_input(char *input)
{
	if (!validate_input(input))
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
	debug_print_with_str("[DEBUG] prepare_input result: ", prepared,
		DEBUG_ENABLED);
	add_history(input);
	if (!validate_input(prepared))
	{
		debug_print("[DEBUG] validate_input returned false", DEBUG_ENABLED);
		free(prepared);
		*status = 0;
		return ;
	}
	debug_print("[DEBUG] validate_input returned true", DEBUG_ENABLED);
	tokens = tokenize(prepared, NULL);
	free(prepared);
	if (!tokens)
	{
		debug_print("[DEBUG] tokenize returned NULL", DEBUG_ENABLED);
		*status = 0;
		return ;
	}
	debug_print("[DEBUG] tokenize succeeded", DEBUG_ENABLED);
	debug_print_tokens(tokens, DEBUG_ENABLED);
	*status = handle_tokens_and_parse(tokens);
}
