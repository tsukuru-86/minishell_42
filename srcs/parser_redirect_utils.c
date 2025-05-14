/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:10:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/14 06:12:16 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

/**
 * @brief 入力リダイレクトを処理します
 */
// static int	open_input_file(const char *filename)
// {
// 	int	fd;

// 	fd = open(filename, O_RDONLY);
// 	if (fd == -1)
// 		perror("minishell: open");
// 	return (fd);
// }

// int	handle_input_redirect(t_command *cmd, char *filename)
// {
// 	int	fd;

// 	(void)cmd;
// 	if (!filename)
// 	{
// 		return (0);
// 	}
// 	fd = open_input_file(filename);
// 	if (fd == -1)
// 		return (0);
// 	if (dup2(fd, STDIN_FILENO) == -1)
// 	{
// 		perror("minishell: dup2");
// 		close(fd);
// 		return (0);
// 	}
// 	close(fd);
// 	return (1);
// }

/**
 * @brief 出力リダイレクトを処理します
 */
// static int	open_output_file(const char *filename, int append)
// {
// 	int	fd;

// 	if (append)
// 		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	else
// 		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd == -1)
// 		perror("minishell: open");
// 	return (fd);
// }

// int	handle_output_redirect(t_command *cmd, char *filename, int append)
// {
// 	int	fd;

// 	(void)cmd;
// 	if (!filename)
// 	{
// 		return (0);
// 	}
// 	fd = open_output_file(filename, append);
// 	if (fd == -1)
// 		return (0);
// 	if (dup2(fd, STDOUT_FILENO) == -1)
// 	{
// 		perror("minishell: dup2");
// 		close(fd);
// 		return (0);
// 	}
// 	close(fd);
// 	return (1);
// }
