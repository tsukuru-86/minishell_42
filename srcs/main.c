/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 00:58:32 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/18 03:22:40 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	initialize_environment(t_minishell *shell, char **envp)
{
	t_env	**env_ptr;

	env_ptr = g_env();
	*env_ptr = create_env_list(envp);
	if (!*env_ptr)
	{
		ft_putstr_fd("minishell: failed to initialize environment\n", 2);
		exit(RET_ERROR);
	}
	shell->env = *env_ptr;
}

static int	handle_input(t_minishell *shell)
{
	char		*input;
	t_token		*tokens;
	t_command	*cmd;
	int			status;

	init_signal_handlers();
	input = readline("minishell > ");
	if (input == NULL)
		return (RET_ERROR);
	if (input && *input)  // 入力が空でない場合のみ履歴に追加
		add_history(input);
	ft_printf_fd(2, "DEBUG INPUT: [%s]\n", input);
	tokens = tokenize(input, shell);
	cmd = parse_tokens(tokens);
	if (cmd != NULL)
	{
		status = execute_command(cmd, shell);
		free_command(cmd);
		free(input);
		return (status);
	}
	free(input);
	return (RET_ERROR);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	shell;
	int			status;

	(void)argc;
	(void)argv;
	ft_memset(&shell, 0, sizeof(t_minishell));
	shell.exit_status = 0;
	initialize_environment(&shell, envp);
	while (1)
	{
		status = handle_input(&shell);
		if (status == -1)
			break ;
	}
	clear_history();
	free_env_list(shell.env);
	return (shell.exit_status);
}
