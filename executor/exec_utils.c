/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 18:48:58 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/12 18:49:03 by uwettasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_commands(t_ast **commands)
{
	if (!commands)
		return ;
	free(commands);
}

void	free_tpp(t_pipe_parameters *tpp, int fd_count)
{
	int	i;

	if (!tpp)
		return ;
	i = 0;
	while (i < fd_count)
	{
		free(tpp->pipefd[i]);
		i++;
	}
	free(tpp->pipefd);
	free(tpp);
}
