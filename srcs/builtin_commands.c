/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025-04-29 10:58:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int builtin_echo(char **args)
{
    int i;
    int newline;

    newline = 1;
    i = 1;
    if (args[1] && ft_strncmp(args[1], "-n", 2) == 0)
    {
        newline = 0;
        i++;
    }
    while (args[i])
    {
        ft_putstr_fd(args[i], 1);
        if (args[i + 1])
            ft_putchar_fd(' ', 1);
        i++;
    }
    if (newline)
        ft_putchar_fd('\n', 1);
    return (0);
}

int builtin_cd(char **args)
{
    char *path;

    if (!args[1])
        path = getenv("HOME");
    else
        path = args[1];
    
    if (chdir(path) == -1)
    {
        ft_putstr_fd("cd: ", 2);
        ft_putstr_fd(path, 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        return (1);
    }
    return (0);
}

int builtin_pwd(char **args)
{
    char cwd[PATH_MAX];
    (void)args;

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        ft_putstr_fd("pwd: error retrieving current directory\n", 2);
        return (1);
    }
    ft_putendl_fd(cwd, 1);
    return (0);
}

int builtin_export(char **args)
{
    int i;

    i = 1;
    if (!args[1])
    {
        ft_putstr_fd("export: not implemented without arguments\n", 2);
        return (1);
    }
    while (args[i])
    {
        if (putenv(args[i]) != 0)
        {
            ft_putstr_fd("export: error setting environment variable\n", 2);
            return (1);
        }
        i++;
    }
    return (0);
}

int builtin_unset(char **args)
{
    int i;

    i = 1;
    if (!args[1])
    {
        ft_putstr_fd("unset: not enough arguments\n", 2);
        return (1);
    }
    while (args[i])
    {
        if (unsetenv(args[i]) != 0)
        {
            ft_putstr_fd("unset: error removing environment variable\n", 2);
            return (1);
        }
        i++;
    }
    return (0);
}

int builtin_env(char **args)
{
    extern char **environ;
    int i;
    (void)args;

    i = 0;
    while (environ[i])
    {
        ft_putendl_fd(environ[i], 1);
        i++;
    }
    return (0);
}

int builtin_exit(char **args)
{
    int status;

    status = 0;
    if (args[1])
    {
        status = ft_atoi(args[1]);
        if (status < 0 || status > 255)
            status = status % 256;
    }
    exit(status);
    return (status);  // This line will never be reached
}