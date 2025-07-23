/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_to_cmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 10:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/23 16:39:12 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/error_messages.h"
#include "minishell.h"
#include "parser.h"
#include "tokenizer/tokenizer.h"
#include "debug/debug.h"

/*
** @brief Check if the first token is not a syntax error
*/
static int	check_trailing_pipe(t_token *tokens)
{
	t_token	*current;
	t_token	*last_non_space;

	if (!tokens)
		return (1);
	last_non_space = NULL;
	current = tokens;
	while (current)
	{
		if (current->type != TOKEN_SPACE && current->type != TOKEN_NEWLINE)
			last_non_space = current;
		current = current->next;
	}
	if (last_non_space && last_non_space->type == TOKEN_PIPE)
	{
		ft_printf_fd(2, ERR_UNEXP_TOKEN, "|");
		return (0);
	}
	return (1);
}

static int	check_syntax_errors(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (!check_trailing_pipe(tokens))
		return (0);
	return (1);
}

/*
** @brief Validate the command structure.
** @param head Head of the command list
** @param tokens Head of the token list
** @return 1 if validation succeeds, 0 if fails
*/
static int	validate_command(t_command *head, t_token *tokens)
{
	if (!check_syntax_errors(tokens))
		return (0);
	if (!check_advanced_syntax(tokens))
		return (0);
	if (head && head->args == NULL && head->redirects == NULL
		&& head->next == NULL && tokens->type == TOKEN_PIPE)
	{
		return (0);
	}
	return (1);
}

/*
** @brief Main loop to convert a token sequence into command structures.
** @param tokens Head of the token list
** @return Head of the converted command list, or NULL on error
*/
static t_command	*parse_tokens_loop(t_token *tokens)
{
	t_command	*cmd;
	t_command	*head;
	t_token		*current;

	cmd = create_command();
	if (!cmd)
	{
		ft_putstr_fd("minishell: failed to create command structure\n", 2);
		return (NULL);
	}
	head = cmd;
	current = tokens;
	while (current)
	{
		if (process_token_in_parse_loop(&cmd, &current, &head) == 0)
			return (NULL);
	}
	return (head);
}

/*
** @brief Convert a token sequence into command structures.
** @param tokens Head of the token list
** @return Head of the converted command list, or NULL on error
*/
t_command	*parse_tokens(t_token *tokens)
{
	t_command	*head;
	t_token		*preprocessed_tokens;
	t_token		*first_token;

	if (!tokens)
		return (NULL);
	preprocessed_tokens = preprocess_tokens(tokens);
	if (!preprocessed_tokens)
	{
		dbg_printf("No tokens remain (empty command)");
		return (create_command());
	}
	first_token = preprocessed_tokens;
	while (first_token && first_token->type == TOKEN_SPACE)
		first_token = first_token->next;
	debug_print_tokens_list(preprocessed_tokens);
	head = parse_tokens_loop(preprocessed_tokens);
	if (!head || !validate_command(head, preprocessed_tokens))
	{
		free_tokens(preprocessed_tokens);
		return (NULL);
	}
	free_tokens(preprocessed_tokens);
	return (head);
}
