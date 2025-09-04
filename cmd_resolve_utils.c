/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_resolve_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 23:42:49 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/03 23:43:04 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_init_access(t_shell *shell, t_cmd_access *access)
{
	char	*path;

	path = ft_get_env_var(shell, "PATH");
	if (path == NULL || path[0] == '\0')
	{
		access->path = false;
	}
	free(path);
}
