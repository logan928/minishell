/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:20:51 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/06 14:24:15 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern sig_atomic_t	g_abort;

void	ft_sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	ft_sigquit_trap(int sig)
{
	(void)sig;
	return ;
}

void	ft_sigint_handler_here(int sig)
{
	(void)sig;
	g_abort = HEREDOC_INT;
	write(STDOUT_FILENO, "\n", 1);
}

void	ft_set_here_sigint(void)
{
	struct sigaction	sa1;

	sa1.sa_handler = ft_sigint_handler_here;
	sigemptyset(&sa1.sa_mask);
	sa1.sa_flags = 0;
	sigaction(SIGINT, &sa1, NULL);
}
