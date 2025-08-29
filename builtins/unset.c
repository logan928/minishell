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
	size_t	i;
	size_t	j;
	size_t	len_var;

	if (!args || !args[0] || !args[1])
		return ;
	j = 1;
	while (args[j] != NULL)
	{
		i = 0;
		while (shell->env[i])
		{
			len_var = ft_strlen(args[j]);
			if (!ft_strncmp(args[j], shell->env[i], len_var)
				&& (shell->env[i][len_var] == '='
				|| shell->env[i][len_var] == '\0'))
				ft_remove_at(shell->env, i);
			else
				i++;
		}
		j++;
	}
}
