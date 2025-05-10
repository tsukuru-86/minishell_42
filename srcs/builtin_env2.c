/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands_env2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/11 01:15:47 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Convert linked list to array
static t_env	**env_to_array(t_env *env, int count)
{
	int		i;
	t_env	**arr;
	t_env	*current;

	i = 0;
	current = env;
	arr = malloc(sizeof(t_env *) * count);
	if (!arr)
		return (NULL);
	while (i < count)
	{
		arr[i] = current;
		current = current->next;
		i++;
	}
	return (arr);
}

// Bubble sort array by name
static void	sort_env_array(t_env **arr, int count)
{
	int		i;
	int		j;
	t_env	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - 1 - i)
		{
			if (ft_strncmp(arr[j]->name, arr[j + 1]->name, PATH_MAX) > 0)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

// Print environment variables in export format
static void	print_env_array(t_env **arr, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(arr[i]->name, 1);
		if (arr[i]->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(arr[i]->value, 1);
			ft_putchar_fd('\"', 1);
		}
		ft_putchar_fd('\n', 1);
		i++;
	}
}

// Print sorted environment variables
static void	print_sorted_env(t_env *env)
{
	int		count;
	t_env	*current;
	t_env	**arr;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	arr = env_to_array(env, count);
	if (!arr)
		return ;
	sort_env_array(arr, count);
	print_env_array(arr, count);
	free(arr);
}

// Export builtin command
int	builtin_export(char **args)
{
	extern t_env	*g_env;
	int				i;
	int				ret;

	if (!args[1])
	{
		print_sorted_env(g_env);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		ret = process_export_arg(args[i], &g_env);
		if (ret)
			return (ret);
		i++;
	}
	return (0);
}
