/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 20:31:12 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/25 17:24:28 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_pwd_inode(char *pwd)
{
	struct stat sb_cwd;
	struct stat sb_pwd;

	if (stat(".", &sb_cwd) == -1 || stat(pwd, &sb_pwd) == -1)
		return (false);
	return (sb_cwd.st_ino == sb_pwd.st_ino && sb_cwd.st_dev == sb_pwd.st_dev);
}

void	ft_set_pwd(t_shell *shell)
{
	char	*pwd;

	pwd = ft_get_env_var(shell, "PWD", 3);
	if (pwd[0] == '\0' || !ft_pwd_inode(pwd))
	{
		free(pwd);
		shell->pwd = ft_get_cwd(shell);
	}
	else
		shell->pwd = pwd;
}

void	ft_init_shell(t_shell *shell, char *envp[])
{
	ft_clone_env(shell, envp);
	if (!shell->env)
		exit(EXIT_FAILURE);
	ft_set_pwd(shell);
	shell->prompt = get_prompt();
	if (!shell->prompt)
	{
		ft_free_arr(shell->env);
		exit(EXIT_FAILURE);
	}
	rl_catch_signals = 0;
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, ft_sigquit_trap);
}

void	ft_critical_error(t_shell *shell)
{
	rl_clear_history();
	if (shell->input)
		free(shell->input);
	if (shell->env)
		ft_free_arr(shell->env);
	ft_free_lexer(shell->lexer);
	if (shell->pwd)
		free(shell->pwd);
	if (shell->prompt)
		free(shell->prompt);
	exit(EXIT_FAILURE);
}
