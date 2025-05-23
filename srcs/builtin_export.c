/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/23 04:52:42 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static void	print_sorted_env(void)
{
	t_env	*current;

	current = get_sorted_env_list();
	print_env_list(current);
}

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
