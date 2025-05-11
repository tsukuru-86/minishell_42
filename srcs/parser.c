/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-11 15:13:41 by tsukuru           #+#    #+#             */
/*   Updated: 2025-05-11 15:13:41 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 新しいコマンド構造体を作成 */
static t_command *create_command(void)
{
    t_command *cmd;

    cmd = (t_command *)malloc(sizeof(t_command));
    if (!cmd)
        return (NULL);
    cmd->args = NULL;
    cmd->redirects = NULL;
    cmd->next = NULL;
    cmd->prev = NULL;
    return (cmd);
}

/* トークンをコマンド引数に追加 */
static int add_argument(t_command *cmd, char *arg)
{
    int i;
    char **new_args;

    // 現在の引数の数を数える
    i = 0;
    if (cmd->args)
    {
        while (cmd->args[i])
            i++;
    }

    // 新しい配列を確保
    new_args = (char **)malloc(sizeof(char *) * (i + 2));
    if (!new_args)
        return (0);

    // 既存の引数をコピー
    i = 0;
    if (cmd->args)
    {
        while (cmd->args[i])
        {
            new_args[i] = cmd->args[i];
            i++;
        }
    }

    // 新しい引数を追加
    new_args[i] = ft_strdup(arg);
    if (!new_args[i])
    {
        free(new_args);
        return (0);
    }
    new_args[i + 1] = NULL;

    // 古い配列を解放して新しい配列に置き換え
    free(cmd->args);
    cmd->args = new_args;
    return (1);
}

/* リダイレクトをコマンドに追加 */
static int add_redirect(t_command *cmd, t_token *token, t_token *next)
{
    t_redirect *redir;
    int type;

    if (!next || next->type != TOKEN_WORD)
        return (0);

    // リダイレクトタイプの判定
    if (token->type == TOKEN_REDIR_IN)
        type = REDIR_IN;
    else if (token->type == TOKEN_REDIR_OUT)
        type = REDIR_OUT;
    else if (token->type == TOKEN_REDIR_APPEND)
        type = REDIR_APPEND;
    else
        return (0);

    // リダイレクト構造体の作成
    redir = create_redirect(type, next->content);
    if (!redir)
        return (0);

    // リダイレクトリストの先頭に追加
    redir->next = cmd->redirects;
    cmd->redirects = redir;
    return (1);
}

/* コマンド構造体の解放 */
void free_command(t_command *cmd)
{
    t_command *next;
    int i;

    while (cmd)
    {
        next = cmd->next;

        // 引数配列の解放
        if (cmd->args)
        {
            i = 0;
            while (cmd->args[i])
                free(cmd->args[i++]);
            free(cmd->args);
        }

        // リダイレクトの解放
        free_redirect(cmd->redirects);

        free(cmd);
        cmd = next;
    }
}

/* トークン列をコマンド構造体に変換 */
t_command *parse_tokens(t_token *tokens)
{
    t_command *cmd;
    t_command *head;
    t_token *current;

    if (!tokens)
        return (NULL);

    cmd = create_command();
    if (!cmd)
        return (NULL);
    head = cmd;

    current = tokens;
    while (current)
    {
        if (current->type == TOKEN_WORD ||
            current->type == TOKEN_SINGLE_QUOTE ||
            current->type == TOKEN_DOUBLE_QUOTE)
        {
            // 引数の追加
            if (!add_argument(cmd, current->content))
            {
                free_command(head);
                return (NULL);
            }
            current = current->next;
        }
        else if (current->type == TOKEN_REDIR_IN ||
                 current->type == TOKEN_REDIR_OUT ||
                 current->type == TOKEN_REDIR_APPEND)
        {
            // リダイレクトの追加
            if (!add_redirect(cmd, current, current->next))
            {
                free_command(head);
                return (NULL);
            }
            current = current->next->next;
        }
        else if (current->type == TOKEN_PIPE)
        {
            // 新しいコマンドの作成
            t_command *new_cmd = create_command();
            if (!new_cmd)
            {
                free_command(head);
                return (NULL);
            }
            new_cmd->prev = cmd;
            cmd->next = new_cmd;
            cmd = new_cmd;
            current = current->next;
        }
        else
            current = current->next;
    }

    return (head);
}