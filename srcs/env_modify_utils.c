// filepath: srcs/env_modify_utils.c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_modify_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:24:50 by muiida            #+#    #+#             */
/*   Updated: 2025/05/20 22:24:50 by muiida            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Assuming free_env_node_internal is defined if needed, or use a direct free.
// For simplicity, let's assume direct free or that free_env_node_internal is available.
static void	free_single_env_node(t_env *node)
{
    if (!node)
        return ;
    free(node->name);
    free(node->value);
    free(node);
}

/* Set or create an environment variable */
// Ensure is_valid_identifier, get_env_node, update_env_value, append_env_node are declared in minishell.h and defined.
int	set_env_node(const char *name, const char *value)
{
    t_env	*node;

    if (!name) // Value can be NULL (empty string)
        return (1); // Error: name is required
    if (!is_valid_identifier(name)) // is_valid_identifier needs to be defined
    {
        ft_putstr_fd("minishell: export: `", 2);
        ft_putstr_fd((char *)name, 2);
        ft_putstr_fd("': not a valid identifier\n", 2);
        return (1); // Error code for invalid identifier
    }
    node = get_env_node(name); // get_env_node is in env_utils.c
    if (node)
    {
        // update_env_value should handle freeing old value and strdup new one
        return (update_env_value(node, value)); // update_env_value needs to be defined
    }
    // append_env_node should create a new node and add it to the list
    return (append_env_node(name, value));   // append_env_node needs to be defined
}

int	remove_env_var(const char *name)
{
    t_env	*current;
    t_env	*prev;
    t_env	**head_ref;

    head_ref = g_env(); // g_env() returns t_env **
    if (!head_ref || !*head_ref || !name)
        return (1); // Or appropriate error for invalid args/empty list
    current = *head_ref;
    prev = NULL;
    while (current)
    {
        if (ft_strncmp(current->name, name, ft_strlen(current->name)) == 0) // Use ft_strcmp for exact match
        {
            if (prev)
                prev->next = current->next;
            else
                *head_ref = current->next; // Update head of the list
            free_single_env_node(current); // Use a helper to free a single node
            return (0); // Success
        }
        prev = current;
        current = current->next;
    }
    return (0); // Not found is not an error for unset in many shells
}