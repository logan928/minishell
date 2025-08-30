/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:56:56 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/29 16:36:54 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_chdir(t_shell *shell, char *path, char *dir)
{
	char	*pwd;

	if (chdir(path) == 0)
	{
		shell->env = ft_strvec_update(shell->env, "OLDPWD", 
				ft_str_join3_cpy_safe(shell, "OLDPWD=", shell->pwd, ""));
		shell->env = ft_strvec_update(shell->env, "PWD",
				ft_str_join3_cpy_safe(shell, "PWD=", path, ""));
		shell->exp = ft_strvec_update(shell->exp, "OLDPWD", 
				ft_str_join3_cpy_safe(shell, "OLDPWD=", shell->pwd, ""));
		shell->exp = ft_strvec_update(shell->exp, "PWD",
				ft_str_join3_cpy_safe(shell, "PWD=", path, ""));
		free(shell->pwd);
		shell->pwd = ft_get_env_var(shell, "PWD");
		if (dir && dir[0] == '-' && dir[1] == '\0')
		{
			pwd = ft_str_join3_cpy_safe(shell, shell->pwd, "\n", "");
			ft_write_safe(shell, pwd, STDOUT_FILENO);
		}
	}
	else
	{
		perror(ft_str_join3_cpy_safe(shell, "minishell: cd: ", dir, ""));
		shell->exit_status = (unsigned char) 1;
	}
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
		*cur = ft_str_join3_cpy_safe(shell, ft_get_pwd(shell), "/", arg);
	else
		*cur = ft_strdup_safe(shell, arg);
}

void	ft_cd(t_shell *shell, char **args)
{
	char	*curpath;
	char	*tmp;

	curpath = NULL;
	if (args[1] != NULL && args[2] != NULL)
		return (ft_too_many_args(shell, "cd", 1));
	if (args[1] == NULL || (args[1][0] == '~' && args[1][1] == '\0'))
	{
		tmp = ft_get_env_var(shell, "HOME");
		if (tmp[0] == '\0')
			return (ft_home_not_set(shell, "cd", tmp));
		curpath = tmp;
		tmp = NULL;
	}
	else
		ft_parse_cd_arg(shell, args[1], &curpath);
	curpath = ft_canonicalize(shell, curpath);
	ft_chdir(shell, curpath, args[1]);
}
