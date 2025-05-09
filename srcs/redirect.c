/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-09 17:08:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025-05-09 17:08:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

t_redirect *create_redirect(int type, char *file)
{
    t_redirect *redirect;

    redirect = (t_redirect *)malloc(sizeof(t_redirect));
    if (!redirect)
        return (NULL);
    redirect->type = type;
    redirect->file = ft_strdup(file);
    if (!redirect->file)
    {
        free(redirect);
        return (NULL);
    }
    redirect->original_fd = -1;
    return (redirect);
}

int setup_redirection(t_redirect *redirect)
{
    int fd;

    if (!redirect)
        return (0);

    // 元のファイルディスクリプタを保存
    if (redirect->type == REDIR_OUT || redirect->type == REDIR_APPEND)
        redirect->original_fd = dup(STDOUT_FILENO);
    else if (redirect->type == REDIR_IN)
        redirect->original_fd = dup(STDIN_FILENO);

    // リダイレクションの種類に応じてファイルを開く
    if (redirect->type == REDIR_OUT)
        fd = open(redirect->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (redirect->type == REDIR_APPEND)
        fd = open(redirect->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (redirect->type == REDIR_IN)
        fd = open(redirect->file, O_RDONLY);
    else
        return (0);

    if (fd == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(redirect->file, 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        return (0);
    }

    // ファイルディスクリプタを複製
    if (redirect->type == REDIR_OUT || redirect->type == REDIR_APPEND)
        dup2(fd, STDOUT_FILENO);
    else if (redirect->type == REDIR_IN)
        dup2(fd, STDIN_FILENO);

    close(fd);
    return (1);
}

void restore_redirection(t_redirect *redirect)
{
    if (!redirect || redirect->original_fd == -1)
        return;

    if (redirect->type == REDIR_OUT || redirect->type == REDIR_APPEND)
        dup2(redirect->original_fd, STDOUT_FILENO);
    else if (redirect->type == REDIR_IN)
        dup2(redirect->original_fd, STDIN_FILENO);

    close(redirect->original_fd);
    redirect->original_fd = -1;
}

void free_redirect(t_redirect *redirect)
{
    if (!redirect)
        return;
    if (redirect->file)
        free(redirect->file);
    if (redirect->original_fd != -1)
        close(redirect->original_fd);
    free(redirect);
}