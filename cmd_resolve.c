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
	ft_memcpy(new, base, len_base);
	ft_memcpy(new + len_base, "/", 1);
	ft_memcpy(new + len_base + 1, cmd, len_cmd);
	new[len_base + len_cmd + 1] = '\0';
	return (new);
}

void	ft_check_cmd(char *cmd, t_cmd_access *cmd_access)
{
	struct stat st;

	if (stat(cmd, &st) == 0)
	{
		cmd_access->exist = true;
		if (S_ISDIR(st.st_mode))
			cmd_access->is_dir = true;
		else if (access(cmd, X_OK) == 0)
			cmd_access->executable = true;
	}
}

bool	ft_try_paths(t_shell *shell, char **paths, char **args, t_cmd_access *cmd_access)
{
	int			i;
	char		*full_path;
	struct stat	st;

	i = 0;
	while (paths[i])
	{
		full_path = ft_combine_path(shell, paths[i], args[0]);
		if (stat(full_path, &st) == 0)
		{
			args[0] = ft_strdup_safe(shell, full_path);
			cmd_access->exist = true;
			if (S_ISDIR(st.st_mode))
				cmd_access->is_dir = true;
			else if (access(full_path, X_OK) == 0)
			cmd_access->executable = true;
		}
		free(full_path);
		if (cmd_access->exist && cmd_access->executable)
			return (true);
		i++;
	}
	return (false);
}

void	ft_build_paths(t_shell *shell, char **args, t_cmd_access *cmd_access)
{
	int		i;
	char	**paths;
	
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], "PATH=", 5) == 0)
		{
			paths = ft_split(shell->env[i] + 5, ':');
			if (paths)
			{
				if (ft_try_paths(shell, paths, args, cmd_access))
				{
					ft_free_arr(paths);
					return ;
				}
				ft_free_arr(paths);
			}
			break ;
		}
		i++;
	}
}

/*
 * Checks if a command exists as an executable in the PATH variable
 * or as an absolute/relative path.
 * 
 * Returns a t_cmd_access struct with access information:
 *   - exist: whether a file exists at the path
 *   - executable: whether the file is executable
 *   - is_dir: whether the path is a directory
 *
 * If multiple matching binaries exist in PATH, only the first
 * executable is returned.
 *
 * Note: executable and is_dir are mutually exclusive.
*/

t_cmd_access	ft_get_cmd_path(t_shell *shell, char **args)
{
	t_cmd_access	cmd_access;

	cmd_access = (t_cmd_access){false, false, false};
	if (!args || !args[0] || args[0][0] == '\0')
		return (cmd_access);
	else if (ft_strchr(args[0], '/'))
	{
		ft_check_cmd(args[0], &cmd_access);
		return (cmd_access);
	}
	ft_build_paths(shell, args, &cmd_access);
	return (cmd_access);
}
