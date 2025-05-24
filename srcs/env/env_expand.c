/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 14:56:05 by tsukuru           #+#    #+#             */
/*   Updated: 2025/05/25 04:57:06 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

/* 文字が環境変数の開始を示すかどうかをチェック */
static int	is_env_var_start(const char *str, int i)
{
	return (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) || str[i
				+ 1] == '_' || str[i + 1] == '?'));
}

/* コンテキスト初期化 */
static void	init_expand_context(t_env_expand_ctx *ctx, const char *str,
		char *res, t_command *cmd)
{
	static int	i;
	static int	j;

	ctx->env_line = str;
	ctx->i = &i;
	ctx->res = res;
	ctx->j = &j;
	ctx->cmd = cmd;
	i = 0;
	j = 0;
}

/* 文字列処理のメインループ */
static void	process_string_chars(t_env_expand_ctx *ctx, int in_dquote)
{
	while (ctx->env_line[*ctx->i])
	{
		if (ctx->env_line[*ctx->i] == '\'' && !in_dquote)
			append_quoted(ctx);
		else if (is_env_var_start(ctx->env_line, *ctx->i))
		{
			if (append_env_str(ctx) < 0)
				return ;
		}
		else
			ctx->res[(*ctx->j)++] = ctx->env_line[(*ctx->i)++];
	}
}

/* Expand environment variables in string */
static char	*expand_env_vars_core(const char *str, int in_dquote,
		t_command *cmd)
{
	t_env_expand_ctx	ctx;
	char				res[4096];

	init_expand_context(&ctx, str, res, cmd);
	process_string_chars(&ctx, in_dquote);
	res[*ctx.j] = '\0';
	return (ft_strdup(res));
}

/* 環境変数展開のメイン関数 */
char	*expand_env_vars(const char *str, int in_dquote, t_command *cmd)
{
	char	*expanded;

	if (!str)
		return (NULL);
	expanded = expand_env_vars_core(str, in_dquote, cmd);
	if (!expanded)
		return (NULL);
	return (expanded);
}
