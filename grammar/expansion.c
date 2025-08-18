/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:22:55 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/18 10:40:10 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_valid_env_char(int c)
{
	if ((c >= 65 && c <= 90)
		|| (c >= 97 && c <= 122)
		|| (c == 95))
		return (1);
	return (0);
}

char	*ft_get_env_var(char *s, size_t len, char **env)
{
	size_t	i = 0;

	i = 0;
	while (env[i] != NULL)
	{
		if (!ft_strncmp(s, env[i], len))
		{
			if (env[i][len] == '=')
				return (ft_strdup(env[i] + len + 1));
		}
		env++;
	}
	return (ft_strdup(""));
}
