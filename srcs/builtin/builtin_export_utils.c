/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 01:15:47 by muiida           #+#    #+#             */
/*   Updated: 2025/05/24 00:00:00 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "builtin_export.h"
#include "error/error_messages.h"
#include "minishell.h"

static int	should_reconstruct_args(char **args, int i)
{
	char	*arg;
	char	*next_arg;
	int		len;

	arg = args[i];
	next_arg = args[i + 1];
	if (!arg || !next_arg)
		return (0);
	len = ft_strlen(arg);
	if (len > 0 && arg[len - 1] == '=')
		return (1);
	if (len > 1 && arg[len - 2] == '+' && arg[len - 1] == '=')
		return (1);
	if (ft_strcmp(next_arg, "=") == 0 || ft_strcmp(next_arg, "+=") == 0)
		return (1);
	return (0);
}

static int	process_export_arg(char *arg)
{
	char	*name;
	char	*value;
	int		append;
	int		ret;

	split_export_arg(arg, &name, &value, &append);
	if (append)
		ret = append_env_value(name, value);
	else
		ret = validate_and_set_env(name, value);
	free(name);
	free(value);
	return (ret);
}

/* Process export arguments with reconstruction for split cases */
int	process_export_with_reconstruction(char **args, int *i)
{
	char	*reconstructed;
	int		next_idx;
	int		ret;

	if (should_reconstruct_args(args, *i))
	{
		reconstructed = reconstruct_split_args(args, *i, &next_idx);
		if (reconstructed)
		{
			ret = process_export_arg(reconstructed);
			free(reconstructed);
			*i = next_idx;
			return (ret);
		}
	}
	ret = process_export_arg(args[*i]);
	(*i)++;
	return (ret);
}

/*
** Remove leading and trailing spaces from a string
*/
char	*ft_trim_spaces(const char *src)
{
	if (!src)
		return (NULL);
	return (ft_strtrim(src, " \t\n\v\f\r"));
}

char	*strip_quotes(char *str)
{
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if ((str[0] == '\'' && str[len - 1] == '\''))
		return (ft_substr(str, 1, len - 2));
	if ((str[0] == '"' && str[len - 1] == '"'))
		return (ft_substr(str, 1, len - 2));
	return (ft_strdup(str));
}
