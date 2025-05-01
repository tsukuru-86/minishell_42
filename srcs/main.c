/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-29 00:58:32 by tsukuru           #+#    #+#             */
/*   Updated: 2025-04-29 00:58:32 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
static volatile sig_atomic_t g_signal = 0;

void signal_handler(int signum)
{
    g_signal = signum;
    if (signum == SIGINT) // Ctrl+C
    {
        write(1, "\n", 1);
        rl_on_new_line();
        // rl_delete_text(0, rl_end);//rl_replace_line("", 0);
        rl_redisplay();
    }
}

void    free_tokens(char **tokens)
{
    int i;

    if (!tokens)
        return;
    i = 0;
    while (tokens[i])
    {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}

static void replace_whitespace(char *str)
{
    while (*str)
    {
        if (*str == '\t' || *str == '\n')
            *str = ' ';
        str++;
    }
}

char **split_command(char *input)
{
    char **tokens;

    if (!input)
        return (NULL);
    replace_whitespace(input);
    tokens = ft_split(input, ' ');
    if (!tokens)
        return (NULL);
    return (tokens);
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    char *input;
    char **args;
    int status;

    status = 0;

    signal(SIGINT, signal_handler);
    signal(SIGQUIT, SIG_IGN);

    while(1)
    {
        g_signal = 0;

        input = readline("minishell > ");
        
        if (!input)
            break;
        if (*input)
            add_history(input);

        args = split_command(input);

        if (args && args[0])
            status = excute_commands(args, envp);

        free(input);
        free_tokens(args);
    }
    clear_history();// rl_clear_history();
    return status;
}