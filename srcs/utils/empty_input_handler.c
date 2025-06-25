/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_input_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 06:33:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/25 21:48:36 by muiida           ###   ########.fr       */
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

	debug_print("[DEBUG] validate_input returned true");
	tokens = tokenize(prepared, NULL);
	if (!tokens)
	{
		debug_print("[DEBUG] tokenize returned NULL");
		return (get_exit_status());
	}
	debug_print("[DEBUG] tokenize succeeded");
	debug_print_tokens(tokens);
	status = handle_tokens_and_parse(tokens);
	return (status);
}

void	process_valid_input(char *input, int *status)
{
	char	*prepared;

	debug_print("[DEBUG] process_valid_input: in");
	debug_print("[DEBUG] process_valid_input: in\n");
	prepared = ft_strdup(input);
	if (!prepared)
		return ;
	if (prepared)
		debug_print_with_str("[DEBUG] after strdup: ", prepared);
	else
		debug_print_with_str("[DEBUG] after strdup: ", "NULL");
	debug_print_with_str("[DEBUG] process_valid_input: prepare_input result: ",
		prepared);
	add_history(input);
	if (!validate_input(prepared))
	{
		debug_print("[DEBUG] process_valid_input:validate_input returned false");
		free(prepared);
		*status = 0;
		return ;
	}
	*status = process_tokenization(prepared);
	free(prepared);
	debug_print("[DEBUG] process_valid_input: out");
}
