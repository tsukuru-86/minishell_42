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


/* コマンドの実行（子プロセス用） */
static int execute_command_in_child(t_command *cmd, char **envp)
{
    int status;

    if (!cmd || !cmd->args || !cmd->args[0])
        exit(0);

    // リダイレクトの設定
    if (cmd->redirects && !setup_redirection(cmd->redirects))
        exit(1);

    // コマンドの実行
    if (is_builtin(cmd->args[0]))
    {
        status = execute_builtin(cmd->args);
        if (cmd->redirects)
            restore_redirection(cmd->redirects);
        exit(status);
    }
    else
    {
        status = execute_external_command(cmd->args, envp);
        exit(status);
    }
}

/* コマンドの実行 */
static int execute_single_command(t_command *cmd, char **envp)
{
    int status;

    if (cmd->redirects && !setup_redirection(cmd->redirects))
        return (1);

    if (is_builtin(cmd->args[0]))
        status = execute_builtin(cmd->args);
    else
    {
        pid_t pid = fork();
        if (pid == -1)
            return (1);
        
        if (pid == 0)
        {
            setup_child_signals();
            status = execute_external_command(cmd->args, envp);
            exit(status);
        }
        waitpid(pid, &status, 0);
        status = WEXITSTATUS(status);
    }

    if (cmd->redirects)
        restore_redirection(cmd->redirects);

    return (status);
}

int excute_commands(t_command *cmd, char **envp)
{
    int status;
    t_command *current;
    int pipeline_result;

    if (!cmd || !cmd->args || !cmd->args[0])
        return (0);

    // シングルコマンドの場合
    if (!cmd->next)
        return execute_single_command(cmd, envp);

    // パイプラインの実行
    pipeline_result = setup_pipeline(cmd);
    if (pipeline_result == 0)
        return (1);  // エラー

    if (pipeline_result == 2)  // 子プロセス
    {
        setup_child_signals();
        execute_command_in_child(cmd, envp);
        // ここには到達しない（子プロセスはexit()で終了）
    }

    // パイプラインの完了を待機
    status = wait_pipeline(cmd);

    // クリーンアップ
    current = cmd;
    while (current)
    {
        cleanup_pipeline(current);
        current = current->next;
    }

    return (status);
}