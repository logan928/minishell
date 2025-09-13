/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:56:56 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/05 15:19:19 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_chdir(t_shell *shell, char *path, char *dir)
{
	char	*pwd;

	if (chdir(path) == 0)
	{
		shell->env = ft_strvec_update(shell->env, "OLDPWD",
				fts_strjoin3cpy(shell, "OLDPWD=", shell->pwd, ""));
		shell->env = ft_strvec_update(shell->env, "PWD",
				fts_strjoin3cpy(shell, "PWD=", path, ""));
		shell->exp = ft_strvec_update(shell->exp, "OLDPWD",
				fts_strjoin3cpy(shell, "OLDPWD=", shell->pwd, ""));
		shell->exp = ft_strvec_update(shell->exp, "PWD",
				fts_strjoin3cpy(shell, "PWD=", path, ""));
		free(shell->pwd);
		shell->pwd = ft_get_env_var(shell, "PWD");
		if (dir && dir[0] == '-' && dir[1] == '\0')
		{
			pwd = fts_strjoin3cpy(shell, shell->pwd, "\n", "");
			fts_write(shell, pwd, STDOUT_FILENO);
		}
	}
	else
		ft_chdir_err(shell, dir);
	free(path);
}

void	ft_parse_cd_arg(t_shell *shell, char *arg, char **cur)
{
	char	*oldpwd;

	if (arg[0] == '-' && arg[1] == '\0')
	{
		oldpwd = ft_get_env_var(shell, "OLDPWD");
		if (oldpwd[0] != '\0')
			*cur = oldpwd;
		else
			free(oldpwd);
	}
	else if (arg[0] != '/')
		*cur = fts_strjoin3cpy(shell, shell->pwd, "/", arg);
	else
		*cur = fts_strdup(shell, arg);
}

bool	ft_cd_home(t_shell *shell, char **cur)
{
	char	*tmp;

	tmp = ft_get_env_var(shell, "HOME");
	if (tmp[0] == '\0')
		return (false);
	*cur = tmp;
	tmp = NULL;
	return (true);
}

bool	ft_cd_oldpwd(t_shell *shell, char **cur)
{
	char	*tmp;

	tmp = ft_get_env_var(shell, "OLDPWD");
	if (tmp[0] == '\0')
		return (false);
	*cur = tmp;
	tmp = NULL;
	return (true);
}

void	ft_cd(t_shell *shell, char **args)
{
	char	*curpath;

	curpath = NULL;
	if (args[1] != NULL && args[2] != NULL)
		return (ft_too_many_args(shell, "cd", 1));
	if (args[1] == NULL || (args[1][0] == '~' && args[1][1] == '\0'))
	{
		if (!ft_cd_home(shell, &curpath))
			return (ft_not_set(shell, "HOME"));
	}
	else if (args[1][0] == '-' && args[1][1] == '\0')
	{
		if (!ft_cd_oldpwd(shell, &curpath))
			return (ft_not_set(shell, "OLDPWD"));
	}
	else
		ft_parse_cd_arg(shell, args[1], &curpath);
	curpath = ft_canonicalize(shell, curpath);
	ft_chdir(shell, curpath, args[1]);
}
