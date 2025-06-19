/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:37:10 by muiida            #+#    #+#             */
/*   Updated: 2025/06/19 17:22:32 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils/debug_utils.h"
#include "utils/input_utils.h"

volatile sig_atomic_t	g_signal = 0;

void	signal_handler(int signum)
{
	g_signal = signum;
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static int	initialize_shell(char **envp)
{
	char	*pwd;

	*get_env_val() = create_env_list(envp);
	if (!get_env_val())
	{
		ft_putstr_fd((char *)"minishell: failed to initialize environment\n",
			STDERR_FILENO);
		return (0);
	}
	set_env_node("?", "0");
	if (!get_env_node("PWD"))
	{
		pwd = getcwd(NULL, 0);
		if (pwd)
		{
			set_env_node("PWD", pwd);
			free(pwd);
		}
	}
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	return (1);
}

void	handle_input(char *input, int *status)
{
	int	empty_status;

	debug_print_with_str("[DEBUG] Enter handle_input: \n", input, DEBUG_ENABLED);
	if (!input)
		return ;
	empty_status = handle_empty_input(input);
	if (empty_status != -1)
	{
		*status = empty_status;
		if (input && *input)
			add_history(input);
		return ;
	}
	debug_print("[DEBUG] Before process_valid_input\n",DEBUG_ENABLED);
	process_valid_input(input, status);
	debug_print("[DEBUG] After process_valid_input\n",DEBUG_ENABLED);
	debug_print("[DEBUG] Exit handle_input\n",DEBUG_ENABLED);
}

int	main(int argc, char **argv, char **envp)
{
	int	status;

	(void)argc;
	(void)argv;
	if (!initialize_shell(envp))
		return (EXIT_FAILURE);
	load_history_file();
	status = main_loop();
	save_history_file();
	rl_clear_history();
	free_env_list();
	return (status);
}
