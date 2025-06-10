/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:42:31 by muiida            #+#    #+#             */
/*   Updated: 2025/06/11 07:38:58 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_export.h"
#include "error/error_messages.h"
#include "minishell.h"

int	append_env_value(const char *name, const char *value)
{
	t_env	*node;
	char	*new_val;

	node = get_env_node(name);
	if (!node)
		return (set_env_node(name, value));
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

int	validate_and_set_env(char *name, char *value)
{
	int		ret;
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
	ret = set_env_node(n, v);
	if (ret != 0)
		ft_printf_fd(STDERR_FILENO, ERR_EXPORT_MALLOC, 2);
	cleanup_strings(n, v);
	return (ret != 0);
}
