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

static t_redirect *parse_redirect(char **args, int *cmd_end)
{
    int i;
    t_redirect *redirect;

    redirect = NULL;
    i = 0;
    *cmd_end = 0;
    while (args[i])
    {
        if (args[i][0] == '>')
        {
            if (args[i][1] == '>')
                redirect = create_redirect(REDIR_APPEND, args[i + 1]);
            else
                redirect = create_redirect(REDIR_OUT, args[i + 1]);
            *cmd_end = i;
            break;
        }
        else if (args[i][0] == '<')
        {
            redirect = create_redirect(REDIR_IN, args[i + 1]);
            *cmd_end = i;
            break;
        }
        i++;
    }
    return (redirect);
}

static char **prepare_command(char **args, int cmd_end)
{
    char **cmd;
    int i;

    if (cmd_end == 0)
        return (args);

    cmd = (char **)malloc(sizeof(char *) * (cmd_end + 1));
    if (!cmd)
        return (NULL);

    i = 0;
    while (i < cmd_end)
    {
        cmd[i] = ft_strdup(args[i]);
        if (!cmd[i])
        {
            while (--i >= 0)
                free(cmd[i]);
            free(cmd);
            return (NULL);
        }
        i++;
    }
    cmd[i] = NULL;
    return (cmd);
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
int excute_commands(char **args, char **envp)
{
    t_redirect *redirect;
    char **cmd;
    int cmd_end;
    int status;

    if (!args || !args[0])
        return 0;

    // リダイレクションの解析
    redirect = parse_redirect(args, &cmd_end);
    cmd = prepare_command(args, cmd_end);
    if (!cmd)
        return 1;

    // リダイレクションの設定
    if (redirect && !setup_redirection(redirect))
    {
        free_redirect(redirect);
        if (cmd != args)
            free_tokens(cmd);
        return 1;
    }

    // コマンドの実行
    if (is_builtin(cmd[0]))
        status = execute_builtin(cmd);
    else
        status = execute_external_command(cmd, envp);

    // リダイレクションの復元と解放
    if (redirect)
    {
        restore_redirection(redirect);
        free_redirect(redirect);
    }
    if (cmd != args)
        free_tokens(cmd);

    return status;
}