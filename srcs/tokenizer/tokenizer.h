/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 08:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/24 05:27:08 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

/* トークナイザーモジュール内部ヘッダー
 * このヘッダーはtokenizerディレクトリ内のファイルでのみ使用
 */

# include "../../minishell.h"

/* トークナイザー内部構造体 */
typedef struct s_tokenizer_stat
{
	char	*word_buf;
	t_token	*tokens;
	int		word_idx;
	int		i;
}	t_tokenizer_stat;

/* トークナイザー内部関数（外部からは呼び出し禁止） */
int		is_delimiter(char c);
int		is_quote(char c);
int		is_meta(char c);
t_token	*create_token(char *content, t_token_type type);
t_token	*create_meta_token(const char *input, int *i);
int		extract_quoted_string(const char *input, int *i, char *word_buf,
			t_token_type *type, t_command *cmd);
int		handle_word_logic(char *input, int *i, t_token **tokens,
			char *word_buf, t_command *cmd);

#endif
