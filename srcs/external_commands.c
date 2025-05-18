/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/19 00:56:11 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Get the PATH environment variable string */
static char	*get_path_env(void)
{
	t_env	*path_env;

	path_env = get_env_node("PATH");
	if (path_env)
		return (path_env->value);
	else
		return (NULL);
}

char	*find_command(char *cmd)
{
	char	*path_env;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = get_path_env();
	if (!path_env)
		return (NULL);
	return (search_in_path(path_env, cmd));
}

static int	count_env_nodes(t_env *env_list)
{
	t_env	*current;
	int		count;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static void	free_env_array(char **env_array, int count)
{
	int	i;

	i = 0;
	while (i < count)
		free(env_array[i++]);
	free(env_array);
}

static char	*create_env_string(t_env *env_node)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(env_node->name, "=");
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, env_node->value);
	free(tmp);
	return (result);
}

char	**env_list_to_array(t_env *env_list)
{
	t_env	*current;
	char	**env_array;
	int		count;
	int		i;

	count = count_env_nodes(env_list);
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	current = env_list;
	while (current)
	{
		env_array[i] = create_env_string(current);
		if (!env_array[i])
		{
			free_env_array(env_array, i);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

static void	launch_child(char *cmd_path, char **args)
{
	char	**env_array;

	env_array = env_list_to_array(*g_env());
	if (!env_array)
	{
		perror("env_list_to_array");
		free(cmd_path);
		exit(1);
	}
	if (execve(cmd_path, args, env_array) == -1)
	{
		perror("execve");
		free(cmd_path);
		free_env_array(env_array, count_env_nodes(*g_env()));
		exit(1);
	}
}

static int	launch_parent(pid_t pid, char *cmd_path)
{
	int	status;

	if (pid < 0)
	{
		perror("fork");
		free(cmd_path);
		return (1);
	}
	waitpid(pid, &status, 0);
	free(cmd_path);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_external_command(char **args)
{
	pid_t	pid;
	char	*cmd_path;

	cmd_path = find_command(args[0]);
	if (!cmd_path)
	{
		ft_putstr_fd((char *)"minishell: command not found: ", 2);
		ft_putendl_fd(args[0], 2);
		return (127);
	}
	pid = fork();
	if (pid == 0)
		launch_child(cmd_path, args);
	return (launch_parent(pid, cmd_path));
}
