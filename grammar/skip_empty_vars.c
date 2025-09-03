/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_empty_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 13:10:49 by mkugan            #+#    #+#             */
/*   Updated: 2025/09/04 01:26:15 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	ft_is_valid_plain_var_name(char *s)
{
	size_t	i;

	if (s == NULL || s[0] == '\0' || s[0] != '$')
		return (false);
	if (!ft_valid_env_first_char(s[1]))
		return (false);
	i = 2;
	while (s[i])
	{
		if (!ft_valid_env_char(s[i]))
			return (false);
		i++;
	}
	return (true);
}

void	ft_skip_empty_vars(t_shell *shell, char **args)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (args[i])
	{
		if (ft_is_valid_plain_var_name(args[i]) && ft_strvec_find(shell->env, args[i] + 1) == -1)
		{
			free(args[i]);
			j = i;
			while (args[j])
			{
				args[j] = args[j + 1];
				j++;
			}
			//continue ;
		}
		i++;
	}
}
