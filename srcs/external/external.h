/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 08:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/11 14:10:12 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXTERNAL_H
# define EXTERNAL_H

# include "../minishell.h"

/* 外部コマンド実行コア関数 */
void	execute_child_process(t_command *cmd);

/* コマンド検索とパス解決 */
char	*find_command_path(char *cmd);
char	*search_in_path(const char *path_env, char *cmd);
char	**env_list_to_array(void);
/* 子プロセス関連ユーティリティ */
void	launch_child(char *cmd_path, char **args);
int		wait_parent(pid_t pid, char *cmd_path);
void	free_env_array(char **env_array, int count);
int		count_env_nodes(t_env *env_list);
int		handle_child_process(char *cmd_path, char **args);
int		handle_fork_error(char *cmd_path);
int		check_if_directory(char *cmd_path, char *cmd_name);
/* 実行補助関数 */
int		handle_command_not_found(char **args);
int		handle_directory_check(char *cmd_path, char **args);
int		execute_external_main(char *cmd_path, char **args);
int		handle_stat_error(char *cmd_name);

#endif
