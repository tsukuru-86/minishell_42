/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 21:46:05 by muiida            #+#    #+#             */
/*   Updated: 2025/05/24 05:50:55 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H
# include "../minishell.h"

typedef struct s_tokenizer_vars
{
	t_token		*tokens;
	char		word_buffer[1024];
	int			i;
	t_command	*cmd_instance;
	int			needs_cmd_free;
}				t_tokenizer_stat;

int				extract_raw_word(const char *input, int *i, char *word_buffer);

int				init_tokenizer_stat(t_tokenizer_stat *vars,
					t_command *cmd_param);
void			cleanup_tokenizer_vars(t_tokenizer_stat *vars);
void			finalize_tokenizer(t_tokenizer_stat *vars);
t_token			*cleanup_and_return_null(t_tokenizer_stat *vars, char *input);
int				handle_word_token_creation(t_tokenizer_stat *vars,
					const char *input);
void			add_token_to_list(t_token **token_list_head,
					t_token *new_token);

int				extract_quoted_string(const char *input, int *i, char *word_buf,
					t_token_type *type, t_command *cmd);
int				handle_word_logic(char *input, int *i, t_token **tokens,
					char *word_buf, t_command *cmd);
int				process_token_segment(char *input, int *i, t_token **tokens,
					t_command *cmd);
t_token			*create_expanded_word_token(char *raw_word, t_command *cmd,
					int *status);
#endif