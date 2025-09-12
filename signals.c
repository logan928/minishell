/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:20:51 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/12 15:34:51 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern sig_atomic_t	g_abort;

void	ft_sigint_main_pre(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	rl_done = 1;
	g_abort = HEREDOC_INT;
}

void	ft_sigint_main_post(int sig)
{
	(void)sig;
	write(STDERR_FILENO, "\n", 1);
}

void	ft_sigquit_post(int sig)
{
	(void)sig;
	write(STDERR_FILENO, "Quit (core dumped)\n", 20);
}

void	ft_sigint_hd_pre(int sig)
{
	ft_sigint_main_pre(sig);
	g_abort = HEREDOC_INT;
}
