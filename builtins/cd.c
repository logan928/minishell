/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:56:56 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/04 17:37:50 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_chdir_err(t_shell *shell, char *dir)
{
	if (chdir(shell->pwd) != 0)
	{
		shell->env = ft_strvec_update(shell->env, "OLDPWD",
				fts_strjoin3cpy(shell, "OLDPWD=", shell->pwd, ""));
		shell->env = ft_strvec_update(shell->env, "PWD",
				fts_strjoin3cpy(shell, "PWD=", shell->pwd, fts_strjoin3cpy(shell, "/", dir, "")));
		shell->exp = ft_strvec_update(shell->exp, "OLDPWD",
				fts_strjoin3cpy(shell, "OLDPWD=", shell->pwd, fts_strjoin3cpy(shell, "/", dir, "")));
		shell->exp = ft_strvec_update(shell->exp, "PWD",
				fts_strjoin3cpy(shell, "PWD=", shell->pwd, dir));
		ft_write_safe(shell, ft_strdup_safe(shell, 
							"cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n"), STDERR_FILENO);
		shell->exit_status = (unsigned char) 0;
		free(shell->pwd);
		shell->pwd = ft_get_env_var(shell, "PWD");
	}
	else
	{
		perror(fts_strjoin3cpy(shell, "minishell: cd: ", dir, ""));
		shell->exit_status = (unsigned char) 1;
	}
}

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
			ft_write_safe(shell, pwd, STDOUT_FILENO);
		}
	}
	else
		ft_chdir_err(shell, dir);
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
		*cur = fts_strjoin3cpy(shell, ft_get_pwd(shell), "/", arg);
	else
		*cur = ft_strdup_safe(shell, arg);
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
	*cur = ft_strdup_safe(shell, tmp);
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
