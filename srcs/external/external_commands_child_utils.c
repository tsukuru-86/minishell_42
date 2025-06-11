/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands_child_utils.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/06/11 12:11:46 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "external.h"
#include "minishell.h"

static char	*create_env_string(t_env *env_node)
{
	char	*result;
	size_t	name_len;
	size_t	value_len;

	if (!env_node || !env_node->name)
		return (NULL);
	name_len = ft_strlen(env_node->name);
	value_len = 0;
	if (env_node->value)
		value_len = ft_strlen(env_node->value);
	result = (char *)malloc(name_len + 1 + value_len + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, env_node->name, name_len + value_len + 2);
	result[name_len] = '=';
	if (env_node->value)
		ft_strlcpy(result + name_len + 1, env_node->value, value_len + 1);
	else
		result[name_len + 1] = '\0';
	return (result);
}

static bool	populate_env_array_from_list(char **env_array, t_env *env_list)
{
	t_env	*current;
	int		i;

	i = 0;
	current = env_list;
	while (current)
	{
		if (current->name)
		{
			env_array[i] = create_env_string(current);
			if (!env_array[i])
			{
				free_env_array(env_array, i);
				return (false);
			}
			i++;
		}
		current = current->next;
	}
	env_array[i] = NULL;
	return (true);
}

/* 環境変数リストを execve で使用できる文字列配列に変換する関数 */
char	**env_list_to_array(void)
{
	char	**env_array;
	int		count;
	t_env	*env_list;

	env_list = *get_env_val();
	count = count_env_nodes(env_list);
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	if (!populate_env_array_from_list(env_array, env_list))
		return (NULL);
	return (env_array);
}

int	wait_parent(pid_t pid, char *cmd_path)
{
	int	status;
	int	exit_status;

	exit_status = 1;
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("minishell: waitpid");
	}
	else
	{
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_status = 128 + WTERMSIG(status);
	}
	free(cmd_path);
	return (exit_status);
}
