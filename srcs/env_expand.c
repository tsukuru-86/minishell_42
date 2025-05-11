/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-11 14:56:05 by tsukuru           #+#    #+#             */
/*   Updated: 2025-05-11 14:56:05 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 環境変数名を抽出 */
static char *extract_env_name(const char *str)
{
    int i;
    char name[1024];

    i = 0;
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
    {
        name[i] = str[i];
        i++;
    }
    name[i] = '\0';
    return (ft_strdup(name));
}

/* 環境変数の値を取得 */
char *expand_env_var(const char *name)
{
    t_env *env_var;
    char *value;

    if (!name || !*name)
        return (ft_strdup("$"));

    env_var = get_env_var(g_env, name);
    if (!env_var)
        return (ft_strdup(""));

    value = ft_strdup(env_var->value);
    if (!value)
        return (NULL);

    return (value);
}

/* 文字列内の環境変数を展開する関数
 * str: 展開対象の文字列
 * in_dquote: ダブルクォート内での処理かどうか
 *
 * 処理内容:
 * 1. シングルクォート内の文字列はそのまま保持（環境変数展開なし）
 * 2. $で始まる環境変数を検出して値に展開
 * 3. その他の文字はそのままコピー
 */
char *expand_env_vars(char *str, int in_dquote)
{
    char result[4096];  // 展開結果用バッファ
    int i;
    int j;
    char *env_name;
    char *env_value;

    i = 0;
    j = 0;
    while (str[i])
    {
        // シングルクォート内の処理
        if (str[i] == '\'' && !in_dquote)
        {
            result[j++] = str[i++];
            while (str[i] && str[i] != '\'')
                result[j++] = str[i++];
            if (str[i])
                result[j++] = str[i++];
        }
        // 環境変数の展開処理
        else if (str[i] == '$' && str[i + 1] &&
                (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
        {
            i++;
            env_name = extract_env_name(str + i);
            if (!env_name)
                return (NULL);
            env_value = expand_env_var(env_name);
            free(env_name);
            if (!env_value)
                return (NULL);
            ft_strlcpy(result + j, env_value, 4096 - j);
            j += ft_strlen(env_value);
            free(env_value);
            while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                i++;
        }
        // 通常文字の処理
        else
            result[j++] = str[i++];
    }
    result[j] = '\0';
    return (ft_strdup(result));
}