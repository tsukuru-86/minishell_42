/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:37:10 by muiida            #+#    #+#             */
/*   Updated: 2025/07/01 23:01:22 by muiida           ###   ########.fr       */
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
		ft_putstr_fd(
			(char *)"minishell: failed to initialize environment\n",
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

static int	handle_empty_input_case(char *input)
{
	int	empty_status;

	empty_status = handle_empty_input(input);
	if (empty_status != -1)
	{
		debug_print_with_int(
			"[DEBUG] Empty command handled, status: ", empty_status);
		if (input && *input)
			add_history(input);
	}
	return (empty_status);
}

void	handle_input(char *input, int *status)
{
	int	empty_status;

	debug_print_with_str("[DEBUG] Enter handle_input: ", input);
	if (!input)
	{
		debug_print("[DEBUG] handle_input: Input is NULL.");
		return ;
	}
	if (!check_input_line_limit(input))
	{
		debug_print("[DEBUG] handle_input: Input line limit exceeded.");
		*status = 1;
		return ;
	}
	empty_status = handle_empty_input_case(input);
	if (empty_status != -1)
	{
		*status = empty_status;
		return ;
	}
	debug_print("[DEBUG] handle_input: Before process_valid_input.");
	process_valid_input(input, status);
	debug_print_with_int(
		"[DEBUG] After process_valid_input, status: ", *status);
	debug_print("[DEBUG] Exit handle_input.");
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

