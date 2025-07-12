/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 08:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/07/13 06:50:55 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "minishell.h"

/* トークナイザーモジュール内部ヘッダー
 * このヘッダーはtokenizerディレクトリ内のファイルでのみ使用
 */

/* トークナイザー内部関数（外部からは呼び出し禁止） */
int		is_delimiter(char c);
int		is_quote(char c);
int		is_meta(char c);
t_token	*create_token(char *content, t_token_type type);
t_token	*safe_create_token(char *content, t_token_type type);
t_token	*create_meta_token(const char *input, int *i);
int		extract_quoted_string(t_tokenizer_stat *stat, const char *input,
			char *word_buf);
void	skip_whitespace(const char *input, int *i);
int		extract_quoted_content(const char *input, int *i, char *buf,
			int *buf_len);
void	extract_word_content(const char *input, int *i, char *buf,
			int *buf_len);
void	add_token_to_list(t_token **token_list_head, t_token *new_token);
t_token	*create_expanded_word_token(char *raw_word, int *status);
t_token	*create_expanded_token(char *buf, t_token_type token_type);
int		extract_raw_word(const char *input, int *i, char *word_buffer);
int		process_token_segment(char *input, int *i, t_token **tokens,
			t_command *cmd);
int		handle_quoted_token_creation(t_tokenizer_stat *vars, const char *input);
int		handle_meta_token_creation(t_tokenizer_stat *vars, const char *input);

int		init_tokenizer_stat(t_tokenizer_stat *vars, t_command *cmd_param);
void	cleanup_tokenizer_vars(t_tokenizer_stat *vars);
void	finalize_tokenizer(t_tokenizer_stat *vars);
t_token	*cleanup_and_return_null(t_tokenizer_stat *vars, char *input);
int		handle_word_token_creation(t_tokenizer_stat *vars, const char *input);
int		handle_space_token_creation(t_tokenizer_stat *vars, const char *input);
int		process_input_char(t_tokenizer_stat *vars, const char *input);
t_token	*get_last_token(t_token *tokens);
int		check_basic_syntax(t_token *tokens);
int		check_advanced_syntax(t_token *tokens);
int		is_pipe_redirect(t_token *prev, t_token *current);
int		validate_redirect_target(t_token *current, t_token *prev);
int		check_pipe_redirect_syntax(t_token *tokens);
void	free_tokens(t_token *tokens);
int		fill_heredoc_quoted(const char *input, int *i, char *word_buffer);
int		fill_heredoc_unquoted(const char *input, int *i, char *word_buffer);
t_token	*create_heredoc_delimiter_token(const char *input, int *i);
#endif
