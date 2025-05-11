/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-25 05:02:31 by tsukuru           #+#    #+#             */
/*   Updated: 2025-04-25 05:02:31 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

/* 単一コマンドの実行 */
static int execute_single_command(t_command *cmd, char **envp)
{
    int status;

    if (!cmd || !cmd->args || !cmd->args[0])
        return (0);

    // リダイレクトの設定
    if (cmd->redirects && !setup_redirection(cmd->redirects))
        return (1);

    // コマンドの実行
    if (is_builtin(cmd->args[0]))
        status = execute_builtin(cmd->args);
    else
        status = execute_external_command(cmd->args, envp);

    // リダイレクトの復元
    if (cmd->redirects)
        restore_redirection(cmd->redirects);

    return (status);
}

int is_builtin(char *cmd)
{
    char *builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL};
    int i;

    i = 0;
    while (builtins[i])
    {
        if (strcmp(cmd, builtins[i]) == 0)
            return 1;
        i++;
    }
    return 0;
}

int execute_builtin(char **args)
{
    if (strcmp(args[0], "echo") == 0)
        return builtin_echo(args);
    else if (strcmp(args[0], "cd") == 0)
        return builtin_cd(args);
    else if (strcmp(args[0], "pwd") == 0)
        return builtin_pwd(args);
    else if (strcmp(args[0], "export") == 0)
        return builtin_export(args);
    else if (strcmp(args[0], "unset") == 0)
        return builtin_unset(args);
    else if (strcmp(args[0], "env") == 0)
        return builtin_env(args);
    else if (strcmp(args[0], "exit") == 0)
        return builtin_exit(args);
    return (1);  // コマンドが見つからない場合
}


/* コマンドの実行 */
int excute_commands(t_command *cmd, char **envp)
{
    int status;

    if (!cmd)
        return (0);

    // パイプラインがない場合は単一コマンドとして実行
    if (!cmd->next)
        return (execute_single_command(cmd, envp));

    // TODO: パイプライン処理の実装（Stage 6で実装予定）
    status = execute_single_command(cmd, envp);

    return (status);
}