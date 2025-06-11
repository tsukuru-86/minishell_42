/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:50:52 by tsukuru           #+#    #+#             */
/*   Updated: 2025/06/11 16:36:08 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "external.h"
#include "minishell.h"

// static char	**ensure_env_vars(char **env_array)
// {
// 	char	*vars[3];
// 	char	*names[3];
// 	int		added;
// 	int		i;

// 	names[0] = "LC_COLLATE";
// 	names[1] = "LANG";
// 	names[2] = "PATH";
// 	added = 0;
// 	i = 0;
// 	while (i < 3)
// 	{
// 		if (!is_env_present(env_array, names[i]))
// 		{
// 			vars[added] = get_env_value_from_list(names[i]);
// 			if (vars[added])
// 				added++;
// 		}
// 		i++;
// 	}
// 	if (added == 0)
// 		return (env_array);
// 	return (add_missing_env_vars(env_array, vars, added));
// }

void	launch_child(char *cmd_path, char **args)
{
	char	**env_array;

	env_array = env_list_to_array();
	if (!env_array)
	{
		perror("minishell: env_list_to_array failed");
		free(cmd_path);
		exit(127);
	}
	execve(cmd_path, args, env_array);
	perror("minishell: execve failed");
	exit(127);
}
