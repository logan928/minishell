/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_resolve.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 12:31:19 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/05 15:08:00 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

# define DEFAULTPATH	"/usr/sbin:/usr/bin:/sbin:/bin"

char	*ft_combine_path(t_shell *shell, char *base, char *cmd)
{
	char	*new;
	size_t	len_base;
	size_t	len_cmd;

	len_base = ft_strlen(base);
	len_cmd = ft_strlen(cmd);
	new = fts_malloc(shell, sizeof(char) * (len_base + len_cmd + 2));
	ft_memcpy(new, base, len_base);
	ft_memcpy(new + len_base, "/", 1);
	ft_memcpy(new + len_base + 1, cmd, len_cmd);
	new[len_base + len_cmd + 1] = '\0';
	return (new);
}

void	ft_check_cmd(t_shell *sh, t_command *cmd, t_cmd_access *cmd_access)
{
	struct stat	st;

	if (stat(cmd->args[0], &st) == 0)
	{
		cmd->path = fts_strdup(sh, cmd->args[0]);
		cmd_access->exist = true;
		if (S_ISDIR(st.st_mode))
			cmd_access->is_dir = true;
		else if (access(cmd->args[0], X_OK) == 0)
			cmd_access->executable = true;
	}
}

bool	ft_try_paths(t_shell *sh, char **paths, t_command *cmd, t_cmd_access *acc)
{
	int			i;
	char		*full_path;
	struct stat	st;

	i = 0;
	while (paths[i])
	{
		full_path = ft_combine_path(sh, paths[i], cmd->args[0]);
		if (stat(full_path, &st) == 0)
		{
			cmd->path = fts_strdup(sh, full_path);
			acc->exist = true;
			if (S_ISDIR(st.st_mode))
				acc->is_dir = true;
			else if (access(full_path, X_OK) == 0)
				acc->executable = true;
		}
		free(full_path);
		if (acc->exist && acc->executable)
			return (true);
		i++;
	}
	return (false);
}

void	ft_build_paths(t_shell *shell, t_command *cmd, t_cmd_access *cmd_access)
{
	char	**paths;
	char	*path;

	path = ft_get_env_var(shell, "PATH");
	if (path == NULL || path[0] == '\0')
	{
		cmd_access->path = false;
		free(path);
		path = fts_strjoin3cpy(shell, DEFAULTPATH, ":", shell->pwd);
	}
	paths = ft_split(path, ':');
	if (paths)
	{
		if (ft_try_paths(shell, paths, cmd, cmd_access))
		{
			free(path);
			ft_free_arr(paths);
			return ;
		}
		free(path);
		ft_free_arr(paths);
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

t_cmd_access	ft_get_cmd_path(t_shell *shell, t_command *cmd)
{
	t_cmd_access	cmd_access;
	struct stat		st;

	cmd_access = (t_cmd_access){false, false, false, true};
	ft_init_access(shell, &cmd_access);
	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
	{
		if (cmd->args[0])
			free(cmd->args[0]);
		cmd->args[0] = fts_strdup(shell, "''");
		return (cmd_access);
	}
	else if (ft_strchr(cmd->args[0], '/'))
	{
		ft_check_cmd(shell, cmd, &cmd_access);
		return (cmd_access);
	}
	else if (stat(cmd->args[0], &st) == 0 && S_ISDIR(st.st_mode))
	{
		cmd_access.exist = true;
		cmd_access.is_dir = true;
		return (cmd_access);
	}
	ft_build_paths(shell, cmd, &cmd_access);
	return (cmd_access);
}
