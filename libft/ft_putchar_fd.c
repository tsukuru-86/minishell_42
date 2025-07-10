/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 17:42:56 by tkomai            #+#    #+#             */
/*   Updated: 2025/07/10 14:59:10 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../srcs/error/error_messages.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

// void test_ft_putchar_fd()
// {
//     // 標準出力に 'A' を出力
//     ft_putchar_fd('A', STDOUT_FILENO);
//     write(STDOUT_FILENO, "\n", 1);  // 改行を出力

//     // 標準エラー出力に 'B' を出力
//     ft_putchar_fd('B', STDERR_FILENO);
//     write(STDERR_FILENO, "\n", 1);  // 改行を出力
// }

// int main()
// {
//     test_ft_putchar_fd();  // テスト関数を呼び出す
//     return (0);
// }