/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_resolve.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 12:31:19 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/22 13:06:34 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_combine_path(t_shell *shell, char *base, char *cmd)
{
	char	*new;
	size_t	len_base;
	size_t	len_cmd;

	len_base = ft_strlen(base);
	len_cmd = ft_strlen(cmd);
	new = ft_malloc_safe(shell, sizeof(char) * (len_base + len_cmd + 2));
	if (!new)
		return (NULL);
	ft_memcpy(new, base, len_base);
	ft_memcpy(new + len_base, "/", 1);
	ft_memcpy(new + len_base + 1, cmd, len_cmd);
	new[len_base + len_cmd + 1] = '\0';
	return (new);
}

bool	ft_check_cmd(char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (true);
	return (false);
}

bool	ft_try_paths(t_shell *shell, char **paths, char **args)
{
	int		i;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		full_path = ft_combine_path(shell, paths[i], args[0]);
		if (!full_path)
			continue ;
		if (access(full_path, X_OK) == 0)
		{
			args[0] = ft_strdup_safe(shell, full_path);
			return (true);
		}
		free(full_path);
		i++;
	}
	return (false);
}

bool	ft_get_cmd_path(t_shell *shell, char **args)
{
	int		i;
	char	**paths;

	if (!args[0] || args[0][0] == '\0')
		return (false);
	if (ft_strchr(args[0], '/'))
		return (ft_check_cmd(args[0]));
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], "PATH=", 5) == 0)
		{
			paths = ft_split(shell->env[i] + 5, ':');
			if (!paths)
				return (false);
			if (ft_try_paths(shell, paths, args))
			{
				ft_free_arr(paths);
				return (true);
			}
			ft_free_arr(paths);
		}
		i++;
	}
	return (false);
}
