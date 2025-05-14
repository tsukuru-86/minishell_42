/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 04:09:56 by muiida            #+#    #+#             */
/*   Updated: 2025/05/14 22:30:26 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
		{
			ft_printf_fd(2, "minishell: export: '%s': not a valid identifier\n",
				(char *)str);
			return (RET_ERROR);
		}
		i++;
	}
	return (RET_SUCCESS);
}

/**
 * @brief 環境変数を検証し、更新または作成します
 */
int	validate_and_update_env(t_env **envs, const char *name, const char *value)
{
	t_env	*e;

	if (is_valid_identifier(name) == RET_ERROR)
		return (RET_ERROR);
	e = get_env_var(*envs, name);
	if (e)
		return (update_env_value(e, value));
	return (append_env_var(envs, name, value));
}
