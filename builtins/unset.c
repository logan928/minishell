/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:31:51 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/29 16:33:40 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_unset(t_shell *shell, char **args)
{
	size_t	j;

	if (!args || !args[0] || !args[1])
		return ;
	j = 1;
	while (args[j] != NULL)
	{
		if (!ft_strvec_remove(shell->env, args[j]))
			shell->exit_status = 1;
		if (!ft_strvec_remove(shell->exp, args[j]))
			shell->exit_status = 1;
		j++;
	}
}
