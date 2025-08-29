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

void	ft_add_env_var(t_shell *shell, char *var, char *val)
{
	char	**new_env;
	size_t	size;

	new_env = ft_realloc_arr(shell->env);
	if (new_env == NULL)
		return ;
	shell->env = new_env;
	size = 0;
	while (shell->env[size])
		size++;
	shell->env[size] = ft_str_join3_cpy_safe(shell, var, "=", val);
}

void	ft_set_env_var(t_shell *shell, char *var, char *val)
{
	size_t	i;
	size_t	len_var;
	bool	found;

	found = false;
	i = 0;
	len_var = ft_strlen(var);
	while (shell->env[i])
	{
		if (!ft_strncmp(var, shell->env[i], len_var))
		{
			if (shell->env[i][len_var] == '=')
			{
				found = true;
				free(shell->env[i]);
				shell->env[i] = ft_str_join3_cpy_safe(shell, var, "=", val);
				return ;
			}
		}
		i++;
	}
	if (!found)
		ft_add_env_var(shell, var, val);
}

void	ft_chdir(t_shell *shell, char *path, char *dir)
{
	char	*pwd;

	if (chdir(path) == 0)
	{
		ft_set_env_var(shell, "OLDPWD", shell->pwd);
		ft_set_env_var(shell, "PWD", path);
		free(shell->pwd);
		shell->pwd = ft_get_env_var(shell, "PWD", 3);
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
		oldpwd = ft_get_env_var(shell, "OLDPWD", 6);
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
		tmp = ft_get_env_var(shell, "HOME", 4);
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
