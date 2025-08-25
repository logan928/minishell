/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 20:31:12 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/25 15:24:44 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

void	ft_init_shell(t_shell *shell, char *envp[])
{
	ft_clone_env(shell, envp);
	if (!shell->env)
		exit(EXIT_FAILURE);
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
	if (shell->prompt)
		free(shell->prompt);
	exit(EXIT_FAILURE);
}
