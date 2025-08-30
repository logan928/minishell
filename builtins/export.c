/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:38:49 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/27 14:01:05 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_print_env_sorted(void)
{
	return ;
}

void	ft_export(t_shell *shell, char **args)
{
	size_t	i;
	char	*equals;

	if (args && args[0] && !args[1])
		ft_print_env_sorted();
	i = 1;
	while (args && args[i])
	{
		equals = ft_strchr(args[i], '=');
		if (equals)
			ft_
	}
}
