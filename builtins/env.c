/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkugan <mkugan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:13:06 by mkugan            #+#    #+#             */
/*   Updated: 2025/08/29 16:37:05 by mkugan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_clone_env(t_shell *shell, char *envp[])
{
	size_t	len;

	if (!envp)
	{
		shell->env = ft_malloc_safe(shell, sizeof(char *));
		shell->env[0] = NULL;
		return ;
	}
	len = 0;
	while (envp[len])
		len++;
	shell->env = ft_malloc_safe(shell, sizeof(char *) * (len + 1));
	shell->env[len] = NULL;
	while (len > 0)
	{
		shell->env[len - 1] = ft_strdup_safe(shell, envp[len - 1]);
		len--;
	}
}

char	**ft_clone_env_and_return(t_shell *shell, char *envp[])
{
	size_t	len;
	char	**env_copy;

	if (!envp)
	{
		env_copy = ft_malloc_safe(shell, sizeof(char *));
		env_copy[0] = NULL;
		return (env_copy);
	}
	len = 0;
	while (envp[len])
		len++;
	env_copy = ft_malloc_safe(shell, sizeof(char *) * (len + 1));
	env_copy[len] = NULL;
	while (len > 0)
	{
		env_copy[len - 1] = ft_strdup_safe(shell, envp[len - 1]);
		len--;
	}
	return (env_copy);
}

void	ft_env(t_shell *shell, char **args)
{
	char	*res;
	char	**e;
	char	*var;

	(void)args;
	e = shell->env;
	res = ft_strdup_safe(shell, "");
	while (*e != NULL)
	{
		var = ft_str_join3_cpy_safe(shell, *e, "\n", "");
		res = ft_strjoin_free_safe(shell, res, var);
		e++;
	}
	ft_write_safe(shell, res, STDOUT_FILENO);
}
