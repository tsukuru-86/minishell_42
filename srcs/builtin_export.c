/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/22 01:23:16 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Convert linked list to array
// static t_env	**env_to_array(t_env *env, int count)
// {
// 	int		i;
// 	t_env	**arr;
// 	t_env	*current;

// 	i = 0;
// 	current = env;
// 	arr = (t_env **)malloc(sizeof(t_env **) * count);
// 	if (!arr)
// 		return (NULL);
// 	while (i < count)
// 	{
// 		arr[i] = current;
// 		current = current->next;
// 		i++;
// 	}
// 	return (arr);
// }

// // Bubble sort array by name
// static void	sort_env_array(t_env **arr, int count)
// {
// 	int		i;
// 	int		j;
// 	t_env	*temp;

// 	i = 0;
// 	while (i < count - 1)
// 	{
// 		j = 0;
// 		while (j < count - 1 - i)
// 		{
// 			if (ft_strncmp(arr[j]->name, arr[j + 1]->name, PATH_MAX) > 0)
// 			{
// 				temp = arr[j];
// 				arr[j] = arr[j + 1];
// 				arr[j + 1] = temp;
// 			}
// 			j++;
// 		}
// 		i++;
// 	}
// }

// Swap nodes in linked list
static t_env	*swap_nodes(t_env *head, t_env *prev, t_env *current,
		t_env *next)
{
	current->next = next->next;
	next->next = current;
	if (prev)
		prev->next = next;
	else
		head = next;
	return (head);
}

// Bubble sort linked list by name
static t_env	*get_sorted_env_list(void)

{
	t_env	*current;
	t_env	*head;
	t_env	*next;

	head = *g_env();
	while (head && head->next)
	{
		current = head;
		next = current->next;
		if (ft_strncmp(current->name, next->name, PATH_MAX) > 0)
			head = swap_nodes(head, NULL, current, next);
		else
			current = current->next;
	}
	return (head);
}

// Print environment variables in export format
static void	print_env_list(t_env *head)
{
	while (head != NULL)
	{
		ft_putstr_fd((char *)"declare -x ", 1);
		ft_putstr_fd(head->name, 1);
		if (head->value)
		{
			ft_putstr_fd((char *)"=\"", 1);
			ft_putstr_fd(head->value, 1);
			ft_putchar_fd('\"', 1);
		}
		ft_putchar_fd('\n', 1);
		head = head->next;
	}
}

// Print environment variables in export format
// static void	print_env_array(t_env **arr, int count)
// {
// 	int	i;

// 	i = 0;
// 	while (i < count)
// 	{
// 		ft_putstr_fd((char *)"declare -x ", 1);
// 		ft_putstr_fd(arr[i]->name, 1);
// 		if (arr[i]->value)
// 		{
// 			ft_putstr_fd((char *)"=\"", 1);
// 			ft_putstr_fd(arr[i]->value, 1);
// 			ft_putchar_fd('\"', 1);
// 		}
// 		ft_putchar_fd('\n', 1);
// 		i++;
// 	}
// }

// Print sorted environment variables
static void	print_sorted_env(void)
{
	t_env	*current;

	current = get_sorted_env_list();
	print_env_list(current);
}

// Export builtin command
int	builtin_export(char **args)
{
	int	i;
	int	ret;

	if (!args[1])
	{
		print_sorted_env();
		return (0);
	}
	i = 1;
	while (args[i])
	{
		ret = process_export_arg(args[i]);
		if (ret)
			return (ret);
		i++;
	}
	return (0);
}
