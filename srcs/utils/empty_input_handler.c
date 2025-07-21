/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_input_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 06:33:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/16 05:36:34 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils/utils.h"
#include "utils/debug.h"

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
		dbg_printf("tokenize returned NULL");
		return (get_exit_status());
	}
	dbg_printf("tokenize succeeded");
	debug_print_tokens_list(tokens);
	status = handle_tokens_and_parse(tokens);
	return (status);
}

static void	process_valid_input_debug(char *input, char *prepared)
{
	(void)input;
	dbg_printf("process_valid_input: in");
	if (prepared)
		dbg_printf("after strdup: %s", prepared);
	else
		dbg_printf("after strdup: NULL");
	dbg_printf("process_valid_input: prepare_input result: %",
		prepared);
}

void	process_valid_input(char *input, int *status)
{
	char	*prepared;

	prepared = ft_strdup(input);
	if (!prepared)
		return ;
	process_valid_input_debug(input, prepared);
	add_history(input);
	if (!validate_input(prepared))
	{
		dbg_printf("[DEBUG] process_valid_input: false");
		free(prepared);
		*status = 0;
		return ;
	}
	*status = process_tokenization(prepared);
	free(prepared);
	dbg_printf("process_valid_input: end");
}
