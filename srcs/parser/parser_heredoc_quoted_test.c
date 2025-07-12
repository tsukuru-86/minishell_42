/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_quoted_test.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:37:10 by muiida            #+#    #+#             */
/*   Updated: 2025/07/13 05:22:55 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils/debug_utils.h"
#include "utils/input_utils.h"
#include "minishell.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

volatile sig_atomic_t	g_signal = 0;

void	signal_handler(int signum)
{
	g_signal = signum;
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_input(char *input, int *status)
{
	int	empty_status;

	debug_print_with_str("[DEBUG] Enter handle_input: \n", input);
	if (!input)
		return ;
	if (!check_input_line_limit(input))
	{
		*status = 1;
		return ;
	}
	empty_status = handle_empty_input(input);
	if (empty_status != -1)
	{
		*status = empty_status;
		if (input && *input)
			add_history(input);
		return ;
	}
	debug_print("[DEBUG] Before process_valid_input\n");
	process_valid_input(input, status);
	debug_print("[DEBUG] After process_valid_input\n");
	debug_print("[DEBUG] Exit handle_input\n");
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_quoted_test.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 04:03:00 by test              #+#    #+#             */
/*   Updated: 2025/07/13 04:25:25 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** 【テスト用】heredocのdelimiter_tokenクオート復元確認
** 本番コードには含めないでください
*/

/*
** テスト用ダミー: heredoc本体を呼ばず即return
*/
int	handle_interactive_heredoc(t_command *cmd, t_token *delimiter_token,
		t_command **head_cmd, t_token **current_token)
{
	(void)cmd;
	(void)delimiter_token;
	(void)head_cmd;
	(void)current_token;
	return (1);
}

int	handle_noninteractive_heredoc(t_command *cmd, t_token *delimiter_token,
		t_command **head_cmd, t_token **current_token)
{
	(void)cmd;
	(void)delimiter_token;
	(void)head_cmd;
	(void)current_token;
	return (1);
}

static t_token	*create_token(const char *content, t_token_type type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->content = ft_strdup(content);
	token->type = type;
	token->next = NULL;
	return (token);
}

static void	test_heredoc_quoted(const char *label, const char *delim,
		t_token_type type)
{
	t_token		*heredoc;
	t_token		*quoted;
	t_token		*current;
	t_command	*cmd;
	t_command	*head_cmd;

	heredoc = create_token("<<", TOKEN_HEREDOC);
	quoted = create_token(delim, type);
	heredoc->next = quoted;
	current = heredoc;
	cmd = create_command();
	head_cmd = NULL;
	printf("%s\n", label);
	handle_heredoc_redirect(cmd, &current, &head_cmd);
	printf("delimiter_token->content: %s\n", quoted->content);
	free_tokens(heredoc);
	free_command(cmd);
}

int	main(void)
{
	test_heredoc_quoted("【D_QUOTED】", "\"EOF\"", TOKEN_D_QUOTED_WORD);
	test_heredoc_quoted("【S_QUOTED】", "'EOF'", TOKEN_S_QUOTED_WORD);
	return (0);
}
