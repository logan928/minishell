/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:58:32 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/25 17:07:43 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_get_cwd(t_shell *shell)
{
	char	*buf;
	size_t	buf_size;
	char	*res;

	buf_size = 1024;
	while (1)
	{
		buf = ft_malloc_safe(shell, buf_size);
		res = getcwd(buf, buf_size);
		if (res != NULL)
			return (buf);
		free(buf);
		if (errno != ERANGE)
			ft_critical_error(shell);
		buf_size *= 2;
	}
}

bool	ft_pwd_inode(char *pwd)
{
	struct stat	sb_cwd;
	struct stat	sb_pwd;

	if (stat(".", &sb_cwd) == -1 || stat(pwd, &sb_pwd) == -1)
		return (false);
	return (sb_cwd.st_ino == sb_pwd.st_ino && sb_cwd.st_dev == sb_pwd.st_dev);
}

void	ft_set_pwd(t_shell *shell)
{
	char	*pwd;

	pwd = ft_get_env_var(shell, "PWD");
	if (pwd[0] == '\0' || !ft_pwd_inode(pwd))
	{
		free(pwd);
		shell->pwd = ft_get_cwd(shell);
	}
	else
		shell->pwd = pwd;
	shell->env = ft_strvec_update(shell->env,
				"PWD", fts_strjoin3cpy(shell, "PWD", "=", shell->pwd));
	shell->exp = ft_strvec_update(shell->exp,
				"PWD", fts_strjoin3cpy(shell, "PWD", "=", shell->pwd));
}

char	*ft_get_pwd(t_shell *shell)
{
	return (ft_strdup_safe(shell, shell->pwd));
}

void	ft_pwd(t_shell *shell, char **args)
{
	(void)args;
	if (shell->pwd)
	{
		printf("%s\n", shell->pwd);
		shell->exit_status = 0;
	}
}
