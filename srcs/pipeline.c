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
static void setup_child_fds(t_command *cmd)
{
    // 入力のリダイレクト
    if (cmd->pipe.read_fd != -1)
    {
        dup2(cmd->pipe.read_fd, STDIN_FILENO);
        close(cmd->pipe.read_fd);
    }

    // 出力のリダイレクト
    if (cmd->pipe.write_fd != -1)
    {
        dup2(cmd->pipe.write_fd, STDOUT_FILENO);
        close(cmd->pipe.write_fd);
    }

    // 使用しないパイプを閉じる
    if (cmd->next)
        close(cmd->next->pipe.read_fd);
}

/* パイプラインのセットアップ */
int setup_pipeline(t_command *cmd)
{
    // パイプラインの初期化
    cmd->pipe.read_fd = -1;
    cmd->pipe.write_fd = -1;
    cmd->pipe.pid = -1;

    // パイプの作成
    if (!setup_pipe_fds(cmd))
        return (0);

    // 子プロセスの作成
    cmd->pipe.pid = fork();
    if (cmd->pipe.pid == -1)
        return (0);

    if (cmd->pipe.pid == 0)
    {
        // 子プロセス
        setup_child_fds(cmd);
        return (2);  // 子プロセスであることを示す特別な値
    }

    // 親プロセス
    // 使用済みのファイルディスクリプタを閉じる
    if (cmd->pipe.read_fd != -1)
        close(cmd->pipe.read_fd);
    if (cmd->pipe.write_fd != -1)
        close(cmd->pipe.write_fd);

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