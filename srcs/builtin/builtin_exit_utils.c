/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 00:26:24 by muiida            #+#    #+#             */
/*   Updated: 2025/06/07 03:03:16 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"
#include "libft.h"
#include "minishell.h"
#include <limits.h>

static long long	convert_to_number(const char *str);

/* 文字列の先頭と末尾のクォートを除去 */
static char	*remove_quotes(const char *str)
{
	int		len;
	int		start;
	int		end;
	char	*result;
	int		i;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	start = 0;
	end = len - 1;
	
	// 先頭のクォートをスキップ
	if (str[start] == '"' || str[start] == '\'')
		start++;
	// 末尾のクォートをスキップ
	if (end > start && (str[end] == '"' || str[end] == '\''))
		end--;
	
	result = malloc(end - start + 2);
	if (!result)
		return (NULL);
	
	i = 0;
	while (start <= end)
		result[i++] = str[start++];
	result[i] = '\0';
	return (result);
}

/* 文字列が有効な数値かどうかをチェック */
int	is_valid_number(const char *str)
{
	char		*clean_str;
	int			i;
	int			has_digits;
	long long	result;

	if (!str || !*str)
		return (0);
	
	clean_str = remove_quotes(str);
	if (!clean_str)
		return (0);
	
	i = 0;
	has_digits = 0;
	
	// 符号をスキップ
	if (clean_str[i] == '-' || clean_str[i] == '+')
		i++;
	
	// 符号だけの場合は無効
	if (!clean_str[i])
	{
		free(clean_str);
		return (0);
	}
	
	// 数字が続くかチェック
	while (clean_str[i])
	{
		if (!ft_isdigit(clean_str[i]))
		{
			free(clean_str);
			return (0);
		}
		has_digits = 1;
		i++;
	}
	
	// オーバーフローチェック
	result = convert_to_number(str);
	if (result == LLONG_MAX || result == LLONG_MIN)
	{
		free(clean_str);
		return (0);
	}
	
	free(clean_str);
	return (has_digits);
}

static long long	convert_to_number(const char *str)
{
	long long	result;
	int			i;
	int			sign;
	char		*clean_str;
	long long	digit;

	clean_str = remove_quotes(str);
	if (!clean_str)
		return (0);
	
	result = 0;
	i = 0;
	sign = 1;

	// 符号をチェック
	if (clean_str[i] == '-' || clean_str[i] == '+')
	{
		if (clean_str[i] == '-')
			sign = -1;
		i++;
	}
	
	// 数値に変換
	while (clean_str[i] && ft_isdigit(clean_str[i]))
	{
		digit = clean_str[i] - '0';
		// オーバーフローチェック
		if (sign == 1 && (result > (LLONG_MAX - digit) / 10))
		{
			free(clean_str);
			return (LLONG_MAX);
		}
		if (sign == -1 && (result > (-(LLONG_MIN + digit)) / 10))
		{
			free(clean_str);
			return (LLONG_MIN);
		}
		result = result * 10 + digit;
		i++;
	}
	
	free(clean_str);
	return (result * sign);
}

int	parse_exit_number(const char *str)
{
	long long	result;
	int			exit_code;

	result = convert_to_number(str);
	
	// bashと同じ動作: 256で割った余りを取る
	// 負の数の場合、C言語の%演算子は負の値を返すことがあるため
	// 正の値に変換する必要がある
	exit_code = (int)(result % 256);
	if (exit_code < 0)
		exit_code += 256;
	
	return (exit_code);
}