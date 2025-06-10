/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand_quote.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 06:55:00 by muiida            #+#    #+#             */
/*   Updated: 2025/06/11 06:57:11 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

static void	process_escape(const char *str, int *i, t_quote_info *info)
{
	(*i)++;
	append_c_escaped(str, i, info->buf, info->k);
	if (str[*i])
		(*i)++;
}

static void	process_dollar(const char *str, int *i, t_quote_info *info)
{
	append_env(str, i, info->buf, info->k);
}

static void	process_char_by_type(const char *str, int *i, t_quote_info *info)
{
	if (info->dollar && str[*i] == '\\')
		process_escape(str, i, info);
	else if (info->dollar && info->quote == '\"' && str[*i] == '$')
		process_dollar(str, i, info);
	else
		info->buf[(*info->k)++] = str[(*i)++];
}

void	process_quote_content(const char *str, int *i, char *buf, int *k)
{
	t_quote_info	info;

	info.quote = str[*i - 1];
	info.dollar = 0;
	if (*i > 1 && str[*i - 2] == '$')
		info.dollar = 1;
	info.buf = buf;
	info.k = k;
	while (str[*i] && str[*i] != info.quote && *k < 4095)
		process_char_by_type(str, i, &info);
}
