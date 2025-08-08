/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:13:06 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/07 17:28:32 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_clone_env(char *envp[])
{
	size_t	len;
	char	**env;

	len = 0;
	while (envp[len])
		len++;
	env = malloc(sizeof(char *) * (len + 1));
	if (!env)
		return (NULL);
	env[len] = NULL;
	while (len > 0)
	{
		env[len - 1] = ft_strdup(envp[len - 1]);
		len--;
	}
	return (env);
}

void	ft_env(char *env[])
{
	while (*env != NULL)
	{
		printf("%s\n", *env);
		env++;
	}
}

void	ft_free_env(char *envp[])
{
	int	i;

	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}
