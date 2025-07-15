/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_command_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:53:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/21 13:16:30 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** @brief Simple copy from standard input to standard output
*/
static void	copy_stdin_to_stdout(void)
{
	char	buffer[1024];
	ssize_t	bytes_read;

	while (1)
	{
		bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
		if (bytes_read <= 0)
			break ;
		if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read)
			break ;
	}
}

/*
** @brief Handler for commands with no arguments and only redirection.
** After a pipe, an empty command outputs standard input as is.
*/
int	handle_empty_command_with_redirects(void)
{
	t_env	*status_node;

	copy_stdin_to_stdout();
	status_node = get_env_node("?");
	if (status_node && status_node->value)
		return (ft_atoi(status_node->value));
	return (0);
}
