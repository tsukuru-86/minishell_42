/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 04:53:10 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/23 00:23:30 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* トークンをリストの末尾に追加する関数。 */

void	add_token_to_list(t_token **token_list_head, t_token *new_token)
{
	t_token	*current;

	if (!token_list_head || !new_token)
		return ;
	if (!(*token_list_head))
	{
		*token_list_head = new_token;
		return ;
	}
	current = *token_list_head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

/* 入力文字列をトークンに分割 */
t_token	*tokenize(char *input, t_command *cmd)
{
	t_token			*tokens;
	char			word[1024];
	int				i;
	int				word_i;
	t_token_type	type;
	t_token			*new_token;
	char			*expanded;
	int				need_free_cmd;

	tokens = NULL;
	need_free_cmd = 0;
	if (!cmd)
	{
		cmd = (t_command *)malloc(sizeof(t_command));
		if (!cmd)
			return (NULL);
		ft_memset(cmd, 0, sizeof(t_command));
		cmd->last_status = 0;
		need_free_cmd = 1;
	}
	i = 0;
	if (!input)
		return (NULL);
	while (input[i])
	{
		if (!process_token_segment(input, &i, &tokens, cmd))
		{
			i++;
			continue ;
		}
		if (is_quote(input[i]))
		{
			if (!extract_quoted_string(input, &i, word, &type, cmd))
			{
				free_tokens(tokens);
				return (NULL);
			}
			new_token = create_token(word, type);
			if (!new_token)
			{
				free_tokens(tokens);
				return (NULL);
			}
			add_token_to_list(&tokens, new_token);
			continue ;
		}
		if (is_meta(input[i]))
		{
			new_token = create_meta_token(input, &i);
			if (!new_token)
			{
				free_tokens(tokens);
				return (NULL);
			}
			add_token_to_list(&tokens, new_token);
			continue ;
		}
		word_i = 0;
		while (input[i] && !is_delimiter(input[i]) && !is_quote(input[i])
			&& !is_meta(input[i]))
			word[word_i++] = input[i++];
		word[word_i] = '\0';
		if (word_i > 0)
		{
			expanded = expand_env_vars(word, 0, cmd);
			if (!expanded)
			{
				free_tokens(tokens);
				return (NULL);
			}
			new_token = create_token(expanded, TOKEN_WORD);
			free(expanded);
			if (!new_token)
			{
				free_tokens(tokens);
				return (NULL);
			}
			add_token_to_list(&tokens, new_token);
		}
	}
	if (need_free_cmd)
		free(cmd);
	return (tokens);
}
