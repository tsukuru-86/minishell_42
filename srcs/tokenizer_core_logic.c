/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_core_logic.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/05/20 22:24:50 by muiida            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Forward declaration for add_token if it's static in tokenizer.c
// Alternatively, pass tokens list head by reference to modify it.
// For simplicity, assuming add_token is available or tokenizer.c handles token list updates.
// The original add_token was static in tokenizer.c.
// To use it here, either make it non-static or pass token list management to tokenizer.c
// Let's assume process_token_segment will create and add tokens itself using create_token and a local add_token logic or by modifying a passed list.
// The original add_token was: static void add_token(t_token **token_list_head, t_token *new_token)
// We will replicate a similar logic or assume create_token returns a token to be added by the caller.

/* 通常の単語を処理し、トークンリストに追加 */
static int	handle_word_logic(char *input, int *i, t_token **tokens, char *word_buf)
{
    int		word_idx;
    char	*expanded_content;
    t_token	*new_token;

    word_idx = 0;
    while (input[*i] && !is_delimiter(input[*i]) && !is_quote(input[*i])
        && !is_meta(input[*i]))
    {
        if (word_idx >= MAX_TOKENS -1) // MAX_TOKENS needs to be defined
        {
            ft_putstr_fd((char *)"minishell: token too long\n", 2);
            return (0); // Error
        }
        word_buf[word_idx++] = input[(*i)++];
    }
    word_buf[word_idx] = '\0';
    if (word_idx > 0)
    {
        expanded_content = expand_env_vars(word_buf, 0); // expand_env_vars needs to be defined
        if (!expanded_content)
            return (0); // Expansion failure
        new_token = create_token(expanded_content, TOKEN_WORD); // create_token is in tokenizer.c
        free(expanded_content);
        if (!new_token)
            return (0); // Token creation failure
        add_token_to_list(tokens, new_token); // add_token_to_list needs to be defined (e.g. in tokenizer.c or here)
    }
    return (1); // Success
}

/* メタ文字を処理し、トークンリストに追加 */
static int	handle_meta_char_logic(char *input, int *i, t_token **tokens)
{
    t_token	*new_token;

    // create_meta_token is in tokenizer_meta_utils.c
    new_token = create_meta_token(input, i); // i is advanced by create_meta_token
    if (!new_token)
    {
        // ft_putstr_fd("minishell: invalid meta character sequence\n", 2); // Optional error
        return (0); // Failure
    }
    add_token_to_list(tokens, new_token); // add_token_to_list
    return (1); // Success
}

/* クォートされた文字列を処理し、トークンリストに追加 */
static int	handle_quoted_string_logic(char *input, int *i, t_token **tokens,
        char *word_buf)
{
    t_token_type	type;
    t_token			*new_token;

    // extract_quoted_string is in tokenizer_quote_processing.c
    // It should fill word_buf and set type.
    if (!extract_quoted_string(input, i, word_buf, &type)) // i is advanced
    {
        // ft_putstr_fd("minishell: unclosed quote or string error\n", 2); // Optional
        return (0); // Error
    }
    // word_buf now contains the (possibly expanded) content.
    new_token = create_token(word_buf, type); // create_token from tokenizer.c
    if (!new_token)
        return (0); // Failure
    add_token_to_list(tokens, new_token); // add_token_to_list
    return (1); // Success
}


/* トークン化のメインループの一部を処理 */
// This function will call the static handlers above.
// It needs access to add_token_to_list, or tokenizer.c handles adding.
// For this structure, let's assume add_token_to_list is available.
int	process_token_segment(char *input, int *i, t_token **tokens)
{
    char	word_buffer[MAX_TOKENS]; // MAX_TOKENS definition needed

    if (is_delimiter(input[*i]))
    {
        (*i)++; // Skip delimiter
        return (1); // Success, continue
    }
    else if (is_quote(input[*i]))
    {
        return (handle_quoted_string_logic(input, i, tokens, word_buffer));
    }
    else if (is_meta(input[*i]))
    {
        return (handle_meta_char_logic(input, i, tokens));
    }
    else // Regular word
    {
        return (handle_word_logic(input, i, tokens, word_buffer));
    }
}