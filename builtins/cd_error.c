/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 14:46:59 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/05 15:04:52 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_chdir_err(t_shell *shell, char *dir)
{
	char *msg;

	if (chdir(shell->pwd) != 0)
	{
		shell->env = ft_strvec_update(shell->env, "OLDPWD",
				fts_strjoin3cpy(shell, "OLDPWD=", shell->pwd, ""));
		shell->env = ft_strvec_update(shell->env, "PWD",
				fts_strjoin3cpy(shell, "PWD=", shell->pwd,
					fts_strjoin3cpy(shell, "/", dir, "")));
		shell->exp = ft_strvec_update(shell->exp, "OLDPWD",
				fts_strjoin3cpy(shell, "OLDPWD=", shell->pwd,
					fts_strjoin3cpy(shell, "/", dir, "")));
		shell->exp = ft_strvec_update(shell->exp, "PWD",
				fts_strjoin3cpy(shell, "PWD=", shell->pwd, dir));
		fts_write(shell, fts_strdup(shell, 
				"cd: error retrieving current directory: getcwd: cannot access parent \
					directories: No such file or directory\n"), STDERR_FILENO);
		shell->exit_status = (unsigned char) 0;
		free(shell->pwd);
		shell->pwd = ft_get_env_var(shell, "PWD");
	}
	else
	{
		msg = fts_strjoin3cpy(shell, "minishell: cd: ", dir, "");
		perror(msg);
		free(msg);
		shell->exit_status = (unsigned char) 1;
	}
}
