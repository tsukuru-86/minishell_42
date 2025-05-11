/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-11 15:36:29 by tsukuru           #+#    #+#             */
/*   Updated: 2025-05-11 15:36:29 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* パイプのファイルディスクリプタを設定 */
static int setup_pipe_fds(t_command *cmd)
{
    int pipefd[2];

    // 次のコマンドがある場合のみパイプを作成
    if (cmd->next)
    {
        if (pipe(pipefd) == -1)
            return (0);
        cmd->pipe.write_fd = pipefd[1];
        cmd->next->pipe.read_fd = pipefd[0];
    }
    return (1);
}

/* 子プロセスのファイルディスクリプタを設定 */
static void setup_child_fds(t_command *cmd, t_command *current)
{
    t_command *tmp;

    // 入力のリダイレクト
    if (current->pipe.read_fd != -1)
    {
        if (dup2(current->pipe.read_fd, STDIN_FILENO) == -1)
            exit(1);
    }

    // 出力のリダイレクト
    if (current->next && current->pipe.write_fd != -1)
    {
        if (dup2(current->pipe.write_fd, STDOUT_FILENO) == -1)
            exit(1);
    }

    // すべてのパイプを閉じる
    tmp = cmd;
    while (tmp)
    {
        if (tmp->pipe.read_fd != -1)
            close(tmp->pipe.read_fd);
        if (tmp->pipe.write_fd != -1)
            close(tmp->pipe.write_fd);
        tmp = tmp->next;
    }
}

/* パイプラインのセットアップ */
int setup_pipeline(t_command *cmd)
{
    t_command *current;
    int pipe_count = 0;
    t_command *tmp = cmd;

    // パイプの数を数える
    while (tmp && tmp->next)
    {
        pipe_count++;
        tmp = tmp->next;
    }

    if (pipe_count == 0)
        return (1);  // パイプがない場合は通常の実行

    // すべてのパイプを作成
    current = cmd;
    while (current && current->next)
    {
        current->pipe.read_fd = -1;
        current->pipe.write_fd = -1;
        current->pipe.pid = -1;
        current->next->pipe.read_fd = -1;
        current->next->pipe.write_fd = -1;
        current->next->pipe.pid = -1;

        if (!setup_pipe_fds(current))
        {
            cleanup_pipeline(cmd);
            return (0);
        }
        current = current->next;
    }

    // 最後のコマンドから順に子プロセスを作成
    current = cmd;
    while (current->next)
        current = current->next;

    while (current)
    {
        current->pipe.pid = fork();
        if (current->pipe.pid == -1)
        {
            cleanup_pipeline(cmd);
            return (0);
        }

        if (current->pipe.pid == 0)
        {
            // 子プロセス
            setup_child_signals();
            setup_child_fds(cmd, current);
            return (2);
        }
        current = current->prev;  // 前のコマンドに移動
    }

    // 親プロセスですべてのパイプを閉じる
    current = cmd;
    while (current)
    {
        if (current->pipe.read_fd != -1)
            close(current->pipe.read_fd);
        if (current->pipe.write_fd != -1)
            close(current->pipe.write_fd);
        current = current->next;
    }

    return (1);
}

/* パイプラインのクリーンアップ */
void cleanup_pipeline(t_command *cmd)
{
    // 残っているファイルディスクリプタを閉じる
    if (cmd->pipe.read_fd != -1)
        close(cmd->pipe.read_fd);
    if (cmd->pipe.write_fd != -1)
        close(cmd->pipe.write_fd);
}

/* パイプラインの完了を待機 */
int wait_pipeline(t_command *cmd)
{
    t_command *current;
    int status;
    int last_status;

    current = cmd;
    last_status = 0;

    while (current)
    {
        if (current->pipe.pid != -1)
        {
            waitpid(current->pipe.pid, &status, 0);
            if (WIFEXITED(status))
                last_status = WEXITSTATUS(status);
        }
        current = current->next;
    }

    return (last_status);
}