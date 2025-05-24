/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 20:46:04 by muiida            #+#    #+#             */
/*   Updated: 2025/05/24 05:43:43 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

/* トークンをリストの末尾に追加する関数。 */
void	add_token_to_list(t_token **token_list_head, t_token *new_token)
{
	t_token	*current;

	if (!token_list_head || !new_token)
		return ;
	if (!(*token_list_head))
	{
		*token_list_head = new_token;
		return ;
	}
	current = *token_list_head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}
