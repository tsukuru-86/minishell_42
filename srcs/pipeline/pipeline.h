/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 02:41:13 by muiida            #+#    #+#             */
/*   Updated: 2025/05/25 04:52:15 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPELINE_H
# define PIPELINE_H
# include "minishell.h"

int		setup_pipeline(t_command *cmd);
void	cleanup_pipeline(t_command *cmd);
void	cleanup_pipeline_commands(t_command *cmd);
int		wait_pipeline(t_command *cmd);
void	pipeline_close_pipes(t_command *cmd);
int		spawn_pipeline_processes(t_command *cmd);
void	init_pipeline(t_command *cmd);
int		create_pipes(t_command *cmd);
void	close_parent_pipes(t_command *cmd);
#endif