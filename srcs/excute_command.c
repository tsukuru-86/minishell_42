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
    if (!args || !args[0])
        return 0;
    
    // ビルトインコマンドの場合
    if (is_builtin(args[0]))
        return execute_builtin(args);
    
    // 外部コマンドの場合
    return execute_external_command(args, envp);
    
    return 0; // ここには到達しない
}