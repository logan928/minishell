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
	size_t	i;

	if (envp == NULL || envp[0] == NULL)
	{
		shell->env = ft_strvec_init(10);
		return ;
	}
	len = 0;
	while (envp[len])
		len++;
	shell->env = ft_strvec_init(len * 2);
	if (!shell->env)
		ft_critical_error(shell);
	i = 0;
	while (i < len)
	{
		if (!ft_strvec_push(&shell->env, ft_strdup_safe(shell, envp[i])))
			ft_critical_error(shell);
		i++;
	}
}

void	ft_env(t_shell *shell, char **args)
{
	char	*res;
	char	**e;
	char	*var;

	(void)args;
	e = shell->env->data;
	res = ft_strdup_safe(shell, "");
	while (*e != NULL)
	{
		var = fts_strjoin3cpy(shell, *e, "\n", "");
		res = ft_strjoin_free_safe(shell, res, var);
		e++;
	}
	ft_write_safe(shell, res, STDOUT_FILENO);
}
