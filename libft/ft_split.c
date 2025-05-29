/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkomai <tkomai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 13:54:12 by tkomai            #+#    #+#             */
/*   Updated: 2024/06/13 16:06:59 by tkomai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	count_box(const char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
		{
			in_word = 0;
		}
		s++;
	}
	return (count);
}

void	free_array(char **array, int size)
{
	while (size--)
	{
		free(array[size]);
	}
	free(array);
}

char	*get_next_word(const char **s, char c)
{
	const char	*start;
	char		*word;

	while (**s && **s == c)
	{
		(*s)++;
	}
	start = *s;
	while (**s && **s != c)
	{
		(*s)++;
	}
	if (start == *s)
		return (NULL);
	word = (char *)malloc((*s - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
	ft_strlcpy(word, start, *s - start + 1);
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		i;
	char	*word;

	if (!s)
		return (NULL);
	result = (char **)malloc((count_box(s, c) + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (*s)
	{
		word = get_next_word(&s, c);
		if (word)
			result[i++] = word;
		else if (*s == '\0')
			break ;
		else
		{
			free_array(result, i);
			return (NULL);
		}
	}
	result[i] = NULL;
	return (result);
}

// int main(void) {
//     char **result;

//     result = ft_split("daddddddddddaaaaaaasss", 'a');
//     if (result == NULL) {
//         printf("No tokens found or memory allocation failed.\n");
//         return (1); // エラーが発生した場合は非0を返して終了
//     }

//     for (int i = 0; result[i]; i++) {
//         printf("%s\n", result[i]);
//         free(result[i]); // 各トークンのメモリを解放します。
//     }
//     free(result); // トークン配列自体のメモ

//     return (0);
// }

// char	**ft_split(char const *s, char c)
// {
// 	char	**result;
// 	int		i;
// 	char	*word;

// 	if (!s)
// 		return (NULL);
// 	result = (char **)malloc((count_box(s, c) + 1) * sizeof(char *));
// 	if (!result)
// 		return (NULL);
// 	i = 0;
// 	while (*s)
// 	{
// 		word = get_next_word(&s, c);
// 		if (word)
// 			result[i++] = word;
// 		else if (*s == '\0')
// 			break ;
// 		else
// 		{
// 			free_array(result, i);
// 			return (NULL);
// 		}
// 	}
// 	result[i] = NULL;
// ↓この部分があるバージョン
// 	if (i == 0 && count_box(s, c) > 0)
// 	{
// 		free(result);
// 		return (NULL);
// 	}
// 	return (result);
// }