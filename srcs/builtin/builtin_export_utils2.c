/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:42:31 by muiida            #+#    #+#             */
/*   Updated: 2025/07/09 02:24:10 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error/error_messages.h"
#include "minishell.h"
#include "builtin.h"

int	append_env_value(const char *name, const char *value)
{
	t_env	*node;
	char	*new_val;

	if (!name || !is_valid_identifier(name))
	{
		if (name)
			ft_printf_fd(STDERR_FILENO, ERR_EXPORT_INVALID_ID, name);
		else
			ft_printf_fd(STDERR_FILENO, ERR_EXPORT_INVALID_ID, "");
		return (1);
	}
	node = get_env_node(name);
	if (!node)
		return (set_env_node_direct(name, value));
	new_val = create_new_value(node->value, value);
	if (!new_val)
		return (1);
	if (node->value)
		free(node->value);
	node->value = new_val;
	return (0);
}

static void	handle_invalid_identifier(char *n, char *v)
{
	if (n)
		ft_printf_fd(STDERR_FILENO, ERR_EXPORT_INVALID_ID, n);
	else
		ft_printf_fd(STDERR_FILENO, ERR_EXPORT_INVALID_ID, "");
	if (n)
		free(n);
	if (v)
		free(v);
}

static void	cleanup_strings(char *n, char *v)
{
	if (n)
		free(n);
	if (v)
		free(v);
}

static int	process_env_setting(char *n, char *v)
{
	int	ret;

	ret = set_env_node_direct(n, v);
	if (ret != 0)
		ft_printf_fd(STDERR_FILENO, ERR_EXPORT_MALLOC, 2);
	cleanup_strings(n, v);
	return (ret);
}

int	validate_and_set_env(char *name, char *value)
{
	char	*n;
	char	*v;

	n = NULL;
	v = NULL;
	if (name)
		n = ft_strdup(name);
	if (value)
		v = ft_strdup(value);
	normalize_export_args(&n, &v);
	if (!is_valid_identifier(n))
	{
		handle_invalid_identifier(n, v);
		return (1);
	}
	return (process_env_setting(n, v));
}
