/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:58:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/12 05:50:38 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "minishell.h"

static int	is_echo_n_option(const char *s)
{
	int	i;

	if (!s || s[0] != '-' || s[1] != 'n')
		return (0);
	i = 2;
	while (s[i] == 'n')
		i++;
	if (s[i] != '\0')
		return (0);
	return (1);
}

static void	putstr_echo(const char *s)
{
	int		i;
	char	c;
	int		in_squote;
	int		in_dquote;

	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (s[i])
	{
		c = s[i];
		if (c == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (c == '\"' && !in_squote)
			in_dquote = !in_dquote;
		else
			ft_putchar_fd(c, 1);
		i++;
	}
}

int	builtin_echo(char **args)
{
	int	newline;
	int	i;
	int	first_arg;

	newline = 1;
	i = 1;
	while (is_echo_n_option(args[i]))
	{
		newline = 0;
		i++;
	}
	first_arg = i;
	while (args[i])
	{
		if (i > first_arg)
			ft_putchar_fd(' ', 1);
		putstr_echo(args[i]);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
	return (0);
}
