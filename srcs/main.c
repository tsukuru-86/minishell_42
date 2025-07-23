/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:37:10 by muiida            #+#    #+#             */
<<<<<<< Updated upstream
/*   Updated: 2025/07/23 18:53:16 by muiida           ###   ########.fr       */
=======
/*   Updated: 2025/07/23 18:22:12 by tkomai           ###   ########.fr       */
>>>>>>> Stashed changes
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils/utils.h"

volatile sig_atomic_t	g_signal = 0;

#ifdef __linux__

void	signal_handler(int signum)
{
	g_signal = signum;
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}
#else

void	signal_handler(int signum)
{
	g_signal = signum;
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}
#endif

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

#ifdef __linux__

int	main(int argc, char **argv, char **envp)
{
	int	status;

	(void)argc;
	(void)argv;
	if (!initialize_shell(envp))
		return (EXIT_FAILURE);
<<<<<<< Updated upstream
=======
	// load_history_file();
>>>>>>> Stashed changes
	status = main_loop();
	rl_clear_history();
	free_env_list();
	return (status);
}
#else

int	main(int argc, char **argv, char **envp)
{
	int	status;

	(void)argc;
	(void)argv;
	if (!initialize_shell(envp))
		return (EXIT_FAILURE);
	status = main_loop();
	clear_history();
	free_env_list();
	return (status);
}
#endif
