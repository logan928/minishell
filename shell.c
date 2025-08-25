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

#include "libft/libft.h"
#include "minishell.h"

void	ft_set_pwd(t_shell *shell)
{
	DIR		*dr;
	char	*pwd;

	pwd = ft_get_env_var(shell, "PWD", 3);
	if (pwd[0] == '\0')
	{
		free(pwd);
		pwd = ft_get_cwd(shell);
	}
	else
	{
		dr = opendir(pwd);
		if (dr == NULL && errno == ENOENT)
		{
			free(pwd);
			pwd = ft_get_cwd(shell);
		}
		else
			free(dr);
	}
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
