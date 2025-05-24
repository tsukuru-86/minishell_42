/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 14:56:05 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/25 03:23:42 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

/* Append quoted section starting at str[*i] */
void	append_quoted(t_env_expand_ctx *ctx)
{
	ctx->res[(*ctx->j)++] = ctx->env_line[(*ctx->i)++];
	while (ctx->env_line[*ctx->i] && ctx->env_line[*ctx->i] != '\'')
		ctx->res[(*ctx->j)++] = ctx->env_line[(*ctx->i)++];
	if (ctx->env_line[*ctx->i])
		ctx->res[(*ctx->j)++] = ctx->env_line[(*ctx->i)++];
}

/* 終了ステータス文字列を結果バッファに追加する */
static int	append_exit_status(t_env_expand_ctx *ctx)
{
	char	*status_str;

	(*(ctx->i))++;
	status_str = ft_itoa(get_exit_status(ctx->cmd));
	if (!status_str)
		return (-1);
	ft_strlcpy(ctx->res + *(ctx->j), status_str, 4096 - *(ctx->j));
	*(ctx->j) += ft_strlen(status_str);
	free(status_str);
	return (0);
}

/* Extract environment variable name from string */
static char	*extract_env_name(const char *str)
{
	int		i;
	char	name[1024];

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		name[i] = str[i];
		i++;
	}
	name[i] = '\0';
	return (ft_strdup(name));
}

/* 通常の環境変数の値を結果バッファに追加する */
static int	append_normal_env(t_env_expand_ctx *ctx)
{
	char	*name;
	char	*value;

	name = extract_env_name(ctx->env_line + *ctx->i);
	if (!name)
	{
		ft_putstr_fd((char *)"minishell: memory allocation error\n", 2);
		return (-1);
	}
	value = expand_env_var(name, ctx->cmd);
	free(name);
	if (!value)
	{
		return (-1);
	}
	ft_strlcpy(ctx->res + *ctx->j, value, 4096 - *ctx->j);
	*ctx->j += ft_strlen(value);
	free(value);
	while (ctx->env_line[*ctx->i] && (ft_isalnum(ctx->env_line[*ctx->i])
			|| ctx->env_line[*ctx->i] == '_'))
		(*ctx->i)++;
	return (0);
}

/* Append environment variable section */
int	append_env_str(t_env_expand_ctx *ctx)
{
	(*ctx->i)++;
	if (ctx->env_line[*ctx->i] == '?')
	{
		if (append_exit_status(ctx) < 0)
			return (-1);
	}
	else
	{
		if (append_normal_env(ctx) < 0)
			return (-1);
	}
	return (0);
}
