/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-11 04:53:10 by tsukuru           #+#    #+#             */
/*   Updated: 2025-05-11 04:53:10 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 新しいトークンを作成 */
static t_token *create_token(char *content, t_token_type type)
{
    t_token *token;

    token = (t_token *)malloc(sizeof(t_token));
    if (!token)
        return NULL;
    token->content = ft_strdup(content);
    if (!token->content)
    {
        free(token);
        return NULL;
    }
    token->type = type;
    token->next = NULL;
    return (token);
}

/* トークンをリストに追加*/
static void add_token(t_token **token, t_token *new_token)
{
    t_token *current;

    if (!(*token))
    {
        *token = new_token;
        return;
    }

    current = *token;
    while (current->next)
        current = current->next;
    current->next = new_token;
}

static int is_delimiter(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

static int is_quote(char c)
{
    return (c == '\'' || c == '\"');
}

static int is_meta(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

/* メタ文字のトークンタイプを判定 */
static t_token_type get_meta_type(char *input, int *i)
{
    if (input[*i] == '|')
    {
        (*i)++;
        return TOKEN_PIPE;
    }
    else if (input[*i] == '<')
    {
        (*i)++;
        return TOKEN_REDIR_IN;
    }
    else if (input[*i] == '>')
    {
        (*i)++;
        if (input[*i] == '>')
        {
            (*i)++;
            return TOKEN_REDIR_APPEND;
        }
        return TOKEN_REDIR_OUT;
    }
    return TOKEN_WORD;  // エラー防止のためのデフォルト値
}

/* メタ文字トークンを作成 */
static t_token *create_meta_token(char *input, int *i)
{
    t_token_type type;
    char meta_str[3];
    int len;

    len = 0;
    meta_str[len++] = input[*i];
    type = get_meta_type(input, i);
    
    if (type == TOKEN_REDIR_APPEND)
        meta_str[len++] = '>';
    
    meta_str[len] = '\0';
    return create_token(meta_str, type);
}

/* クォートされた文字列を抽出 */
static int extract_quoted_string(char *input, int *i, char *word, t_token_type *type)
{
    char quote_char;
    int word_i;

    quote_char = input[*i];
    (*i)++;  // クォート文字をスキップ
    word_i = 0;

    *type = (quote_char == '\'') ? TOKEN_SINGLE_QUOTE : TOKEN_DOUBLE_QUOTE;

    while (input[*i] && input[*i] != quote_char)
    {
        word[word_i++] = input[*i];
        (*i)++;
    }

    if (input[*i] == quote_char)
    {
        (*i)++;  // 終端のクォート文字をスキップ
        word[word_i] = '\0';

        // 環境変数の展開（シングルクォート内では展開しない）
        if (*type == TOKEN_DOUBLE_QUOTE)
        {
            char *expanded = expand_env_vars(word, 1);
            if (!expanded)
                return (0);
            ft_strlcpy(word, expanded, 1024);
            free(expanded);
        }
        return (1);
    }
    return (0);  // クォートが閉じられていない
}

/* 入力文字列をトークンに分割 */
t_token *tokenize(char *input)
{
    t_token *tokens;
    char word[1024];
    int i;
    int word_i;
    t_token_type type;

    tokens = NULL;
    i = 0;
    while (input[i])
    {
        // スペースをスキップ
        if (is_delimiter(input[i]))
        {
            i++;
            continue;
        }

        // クォートの処理
        if (is_quote(input[i]))
        {
            if (!extract_quoted_string(input, &i, word, &type))
            {
                // クォートが閉じられていない場合のエラー処理
                free_tokens(tokens);
                return (NULL);
            }
            t_token *new_token = create_token(word, type);
            if (!new_token)
            {
                free_tokens(tokens);
                return (NULL);
            }
            add_token(&tokens, new_token);
            continue;
        }

        // メタ文字の処理
        if (is_meta(input[i]))
        {
            t_token *new_token = create_meta_token(input, &i);
            if (!new_token)
            {
                free_tokens(tokens);
                return (NULL);
            }
            add_token(&tokens, new_token);
            continue;
        }

        // 通常の単語を抽出
        word_i = 0;
        while (input[i] && !is_delimiter(input[i]) && !is_quote(input[i]) && !is_meta(input[i]))
            word[word_i++] = input[i++];
        word[word_i] = '\0';

        // トークンを作成して追加
        if (word_i > 0)
        {
            // 環境変数の展開
            char *expanded = expand_env_vars(word, 0);
            if (!expanded)
            {
                free_tokens(tokens);
                return (NULL);
            }

            t_token *new_token = create_token(expanded, TOKEN_WORD);
            free(expanded);
            if (!new_token)
            {
                free_tokens(tokens);
                return (NULL);
            }
            add_token(&tokens, new_token);
        }
    }
    return (tokens);
}

/* トークンリストの解放 */
void free_tokens(t_token *tokens)
{
    t_token *current;
    t_token *next;

    current = tokens;
    while (current)
    {
        next = current->next;
        free(current->content);
        free(current);
        current = next;
    }
}

/* デバッグ用：トークンの内容を表示 */
void print_tokens(t_token *tokens)
{
    t_token *current;
    int     i;

    i = 0;
    current = tokens;
    while (current)
    {
        printf("Token %d: [%s] (type: %d)\n",
            i++, current->content, current->type);
        current = current->next;
    }
}